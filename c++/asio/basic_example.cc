#include <boost/asio.hpp>
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void background_task(boost::asio::any_io_executor ex) {
    boost::asio::steady_timer timer(ex, std::chrono::seconds(2));

    timer.async_wait([](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "The timer went off..." << std::endl;
        }
    });


    boost::asio::post(ex, []() {
        for (int i = 0; i < 10000; ++i) {
            std::cout << " Posting the easiest task in the whole world..." << std::endl;
        }
    });
}

int main() {
    boost::asio::thread_pool pool(4);

    boost::asio::post(pool, [&pool] {
        background_task(pool.get_executor());
    });

    pool.join();

    std::cout << "Background task completed!!!" << std::endl;
    return 0;
}
