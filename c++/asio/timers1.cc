#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>
#include <memory>

void timer_handle(const boost::system::error_code& ec) {
    if (!ec) {
        std::cout << "Timer expired!!!" << std::endl;
    } else {
        std::cerr << "Error in timer: " << ec.message() << std::endl;
    }
}

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, std::chrono::seconds(1));

    timer.async_wait(&timer_handle);
    io_context.run();
    return 0;
}
