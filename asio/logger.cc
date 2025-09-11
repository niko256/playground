#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/thread_pool.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <thread>

const std::string LOG_FILENAME = "log.txt";
const int NUM_MESSAGES_PER_THREAD = 5;
const size_t NUM_THREADS = 4;

class Logger {
  private:
    boost::asio::io_context::strand strand_;
    std::ofstream file_;

  public:
    Logger(boost::asio::io_context& io_ctx, std::string& filename)
        : strand_(io_ctx), file_(filename, std::ios::out | std::ios::app) {

        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open log file...");
        }
    }

    void log(std::string log_message) {
        strand_.post([this, log_message]() {
            do_log(log_message);
        });
    }

  private:
    void do_log(std::string log_message) {
        file_ << log_message << std::endl;
    }
};

void worker(std::shared_ptr<Logger> logger, int thread_id) {
    for (size_t i = 0; i < NUM_MESSAGES_PER_THREAD; ++i) {
        std::ostringstream oss;

        oss << "Thread " << thread_id << " logging message " << i << std::endl;
        logger->log(oss.str());

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::thread_pool pool(4);

        auto work_guard = boost::asio::make_work_guard(io_context);
        std::shared_ptr<Logger> logger = std::make_shared<Logger>(io_context, LOG_FILENAME);

        std::cout << "[LOGGING] " << NUM_MESSAGES_PER_THREAD << " messages from " <<
    } catch () {
    }
}
