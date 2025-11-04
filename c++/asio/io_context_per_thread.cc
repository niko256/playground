#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <iostream>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

void background_task(size_t i) {
    sync_cout << "Thread: " << i << ": Starting..." << std::endl;
    boost::asio::io_context io_ctx;

    auto work_guard = boost::asio::make_work_guard(io_ctx);

    sync_cout << "Thread: " << i << " is setting up new timer..." << std::endl;

    boost::asio::steady_timer timer(io_ctx, 1s);

    timer.async_wait([&](const boost::system::error_code& ec) {
        if (!ec) {
            sync_cout << "Timer expired successfully!!!" << std::endl;
        } else {
            sync_cout << "Timer got an error: " << ec.message() << std::endl;
        }

        work_guard.reset();
    });

    sync_cout << "Thread " << i << " is running io_context right now..." << std::endl;

    io_ctx.run();
}

int main() {
    const size_t NUM_THREADS = 4;
    std::vector<std::jthread> threads;

    for (auto i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(background_task, i);
    }
    return 0;
}
