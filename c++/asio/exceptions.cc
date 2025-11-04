#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>


using namespace std::chrono_literals;

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, 6s);

    auto fut = timer.async_wait(boost::asio::use_future);

    std::jthread io_thread([&io_context]() {
        io_context.run();
    });

    std::this_thread::sleep_for(3s);

    timer.cancel();

    try {
        fut.get();
        std::cout << "Timer expired successfully!" << std::endl;
    } catch (const boost::system::system_error& se) {
        std::cerr << "Timer failed: " << se.code().message() << std::endl;
    }
    return 0;
}
