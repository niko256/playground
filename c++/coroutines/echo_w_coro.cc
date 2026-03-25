#include <boost/asio.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <exception>
#include <iostream>
#include <string>

/**
 * Asynchronous TCP Echo Server
 */

using boost::asio::ip::tcp;
namespace asio = boost::asio;

/** Server configuration constants */
constexpr size_t BUFFER_SIZE = 1024;
constexpr unsigned short DEFAULT_PORT = 12345;
constexpr size_t NUM_THREADS = 4;

/**
 * Handle a single client connection using coroutines.
 * Reads data from client and echoes it back.
 *
 * @param socket Connected client socket (moved)
 * @return Awaitable coroutine result
 */
asio::awaitable<void> handle_client(tcp::socket socket) {
    try {
        /** Get client endpoint for logging */
        auto remote_ep = socket.remote_endpoint();
        std::cout << "[" << remote_ep << "] Client connected\n";

        /** Buffer for reading client data */
        char data[BUFFER_SIZE];

        while (true) {
            /** Read data from client */
            size_t bytes_read = co_await socket.async_read_some(
                asio::buffer(data),
                asio::use_awaitable);

            /** Check for connection close */
            if (bytes_read == 0) {
                std::cout << "[" << remote_ep << "] Client disconnected\n";
                break;
            }

            /** Process received data */
            std::string received(data, bytes_read);

            /** Remove trailing newline/carriage return for command processing */
            while (!received.empty() &&
                   (received.back() == '\n' || received.back() == '\r')) {
                received.pop_back();
            }

            std::cout << "[" << remote_ep << "] Received: " << received << "\n";

            /** Check for QUIT command */
            if (received == "QUIT") {
                const std::string goodbye = "Goodbye!\r\n";
                co_await asio::async_write(
                    socket,
                    asio::buffer(goodbye),
                    asio::use_awaitable);
                std::cout << "[" << remote_ep << "] Client requested disconnect\n";
                break;
            }

            /** Echo data back to client */
            co_await asio::async_write(
                socket,
                asio::buffer(data, bytes_read),
                asio::use_awaitable);
        }

    } catch (std::exception& e) {
        /** Log any errors that occur during client handling */
        std::cerr << "Client error: " << e.what() << "\n";
    }

    /** Socket automatically closes when coroutine exits */
}

/**
 * Accept incoming connections and spawn handlers.
 *
 * @param io IO context for async operations
 * @param port Port number to listen on
 * @return Awaitable coroutine result
 */
asio::awaitable<void> accept_connections(asio::io_context& io, unsigned short port) {
    /** Create acceptor and start listening */
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));

    /** Enable SO_REUSEADDR to avoid "Address already in use" errors */
    acceptor.set_option(tcp::acceptor::reuse_address(true));

    std::cout << "Server listening on port " << port << "\n";

    while (true) {
        try {
            /** Accept new client connection */
            tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);

            /** Spawn new coroutine to handle this client */
            asio::co_spawn(
                io,
                handle_client(std::move(socket)),
                asio::detached);

        } catch (std::exception& e) {
            /** Log accept errors but continue listening */
            std::cerr << "Accept error: " << e.what() << "\n";
        }
    }
}

int main() {
    try {
        asio::io_context io_context;
        auto work_quard = asio::make_work_guard(io_context);


        /** Set up signal handling for graceful shutdown */
        asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context](auto, auto) {
            std::cout << "\nShutdown signal received, stopping server...\n";
            io_context.stop();
        });

        /** Start the acceptor coroutine */
        asio::co_spawn(
            io_context,
            accept_connections(io_context, DEFAULT_PORT),
            asio::detached);

        /** Create thread pool for running io_context */
        asio::thread_pool pool(NUM_THREADS);

        for (size_t i = 0; i < NUM_THREADS; ++i) {
            asio::post(pool, [&io_context]() {
                io_context.run();
            });
        }

        std::cout << "Server started with " << NUM_THREADS << " worker threads\n";
        std::cout << "Press Ctrl+C to stop\n\n";

        /** Wait for all threads to complete */
        pool.join();

        std::cout << "Server stopped\n";

    } catch (std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
