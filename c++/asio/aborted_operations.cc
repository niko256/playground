#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void handle_timeout(const boost::system::error_code& ec) {
    if (ec == boost::asio::error::operation_aborted) {
        std::cout << "Timer canceled!!!" << std::endl;
    } else if (!ec) {
        std::cout << "Timer expired!!!" << std::endl;
    } else {
        std::cout << "Error occured : " << ec.message() << std::endl;
    }
}

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, 5s);

    timer.async_wait(handle_timeout);
    std::this_thread::sleep_for(2s);
    timer.cancel();

    io_context.run();

    return 0;
}
