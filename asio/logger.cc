#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/thread_pool.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <thread>

/**
 * STRAND = EXECUTOR LOGIC + SERIALIZATION MECHANISM
 *
 * This example demonstrates how strand ensures thread-safe logging
 * in a multi-threaded TCP server without using explicit locks.
 */

namespace asio = boost::asio;
using asio::ip::tcp;

/**
 * Thread-safe asynchronous logger using strand for serialization.
 *
 * The logger can be called from any thread, but all file writes
 * are serialized through a strand to prevent data races.
 */
class AsyncLogger {
  private:
    /** Strand ensures all logging operations are serialized */
    asio::strand<asio::io_context::executor_type> strand_;

    /** Reference to io_context for async operations */
    asio::io_context& io_;

    /** Output file stream */
    std::ofstream file_;

    /** Queue of messages waiting to be written */
    std::queue<std::string> pending_writes_;

    /** Flag to track if a write operation is in progress */
    bool write_in_progress_ = false;

    /**
     * Generate timestamp string for log entries
     * @return Formatted timestamp string
     */
    std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    /**
     * Process pending writes from the queue.
     * This function is always called within the strand context,
     * so no additional synchronization is needed.
     */
    void do_write() {
        if (pending_writes_.empty()) {
            write_in_progress_ = false;
            return;
        }

        const auto& msg = pending_writes_.front();
        file_ << msg << std::flush;
        pending_writes_.pop();

        /** Schedule next write within the same strand */
        asio::post(strand_, [this] { do_write(); });
    }

  public:
    /**
     * Construct AsyncLogger
     * @param io IO context for async operations
     * @param filename Path to log file
     * @throws std::runtime_error if file cannot be opened
     */
    AsyncLogger(asio::io_context& io, const std::string& filename)
        : io_(io),
          strand_(asio::make_strand(io)),
          file_(filename, std::ios::app) {
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open log file");
        }
    }

    /**
     * Log a message asynchronously.
     * Thread-safe: can be called from any thread.
     *
     * @param level Log level (INFO, ERROR, DEBUG, etc.)
     * @param message Message to log
     */
    void log(const std::string& level, const std::string& message) {
        std::string full_msg = "[" + timestamp() + "] [" + level + "] " + message + "\n";

        /**
         * Post to strand ensures this lambda executes serially
         * with respect to other strand operations
         */
        asio::post(strand_, [this, msg = std::move(full_msg)] {
            pending_writes_.push(msg);

            if (!write_in_progress_) {
                write_in_progress_ = true;
                do_write();
            }
        });
    }

    /** Convenience methods for different log levels */
    void info(const std::string& msg) { log("INFO", msg); }

    void error(const std::string& msg) { log("ERROR", msg); }

    void debug(const std::string& msg) { log("DEBUG", msg); }
};

/**
 * TCP session handler for echo server.
 * Each session represents a single client connection.
 */
class Session : public std::enable_shared_from_this<Session> {
  private:
    /** Client socket */
    tcp::socket socket_;

    /** Reference to shared logger */
    AsyncLogger& logger_;

    /** Buffer for read/write operations */
    std::array<char, 1024> buffer_;

    /** Client address for logging */
    std::string client_addr_;

  public:
    /**
     * Construct a new session
     * @param socket Connected client socket (moved)
     * @param logger Reference to async logger
     */
    Session(tcp::socket socket, AsyncLogger& logger)
        : socket_(std::move(socket)),
          logger_(logger) {
        client_addr_ = socket_.remote_endpoint().address().to_string();
    }

    /**
     * Start the session by initiating the read loop
     */
    void start() {
        logger_.info("New connection from " + client_addr_);
        do_read();
    }

  private:
    /**
     * Asynchronously read data from client
     */
    void do_read() {
        auto self = shared_from_this();

        socket_.async_read_some(
            asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    std::string data(buffer_.data(), length);
                    logger_.debug("Received from " + client_addr_ + ": " + data);
                    do_write(length);
                } else {
                    logger_.info("Connection closed: " + client_addr_);
                }
            });
    }

    /**
     * Asynchronously write data back to client (echo)
     * @param length Number of bytes to write
     */
    void do_write(std::size_t length) {
        auto self = shared_from_this();

        asio::async_write(
            socket_,
            asio::buffer(buffer_, length),
            [this, self](boost::system::error_code ec, std::size_t bytes) {
                if (!ec) {
                    logger_.debug("Sent " + std::to_string(bytes) + " bytes to " + client_addr_);
                    do_read();
                } else {
                    logger_.error("Write error to " + client_addr_ + ": " + ec.message());
                }
            });
    }
};

/**
 * TCP server that accepts client connections
 */
class Server {
  private:
    /** IO context reference */
    asio::io_context& io_;

    /** TCP acceptor for incoming connections */
    tcp::acceptor acceptor_;

    /** Logger reference */
    AsyncLogger& logger_;

    /**
     * Asynchronously accept new connections
     */
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    /** Create new session for each client */
                    std::make_shared<Session>(std::move(socket), logger_)->start();
                } else {
                    logger_.error("Accept error: " + ec.message());
                }
                /** Continue accepting new connections */
                do_accept();
            });
    }

  public:
    /**
     * Construct server
     * @param io IO context for async operations
     * @param port Port to listen on
     * @param logger Logger instance
     */
    Server(asio::io_context& io, unsigned short port, AsyncLogger& logger)
        : io_(io),
          acceptor_(io, tcp::endpoint(tcp::v4(), port)),
          logger_(logger) {
        logger_.info("Server started on port " + std::to_string(port));
        do_accept();
    }
};

int main() {
    try {
        /** Main IO context for all async operations */
        asio::io_context io;

        /** Work guard prevents io.run() from returning when there's no work */
        auto work = boost::asio::make_work_guard(io);

        /** Number of worker threads */
        constexpr size_t NUM_THREADS = 4;

        /** Create async logger */
        AsyncLogger logger(io, "server.log");

        /** Create and start server */
        Server server(io, 8080, logger);

        /** Create a thread pool with four worker-threads */
        asio::thread_pool pool(NUM_THREADS);

        for (size_t i = 0; i < NUM_THREADS; ++i) {
            asio::post(pool, [&io] {
                io.run();
            });
        }

        /** Wait for shutdown signal or gracefull shutdown
         * for now, just wait forever
         */
        pool.join();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
