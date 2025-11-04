#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>
#include <syncstream>
#include <thread>


using namespace std::chrono_literals;

#define sync_cout std::osyncstream(std::cout)

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, 2s);

    timer.async_wait([](const boost::system::error_code& ec) {
        sync_cout << "Timer expired!!!" << std::endl;
    });

    const int NUM_THREADS = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&io_context]() {
            io_context.run();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
