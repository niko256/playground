#include <boost/asio.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <iostream>

/**
 * @brief Simple example of nested dispatch() policy of handle
 *
 * @return just basic int (as usual main foo)
 */
int main() {
    boost::asio::io_context io_context;

    boost::asio::post(io_context, []() {
        std::cout << "post() handler#1 (from queue)" << std::endl;
    });


    boost::asio::post(io_context, []() {
        std::cout << "another post() handler (also from queue)" << std::endl;
    });


    boost::asio::dispatch(io_context, [&io_context]() {
        std::cout << "runs immidiately...!!!!!!" << std::endl;
    });

    boost::asio::post(io_context, [&io_context]() {
        std::cout << "post() handler#3 (which call nested dispatch internally)" << std::endl;

        boost::asio::dispatch(io_context, []() {
            std::cout << "dispatch handle (executes immidiately)" << std::endl;
        });
        std::cout << "post() handler#3 finished!" << std::endl;
    });

    boost::asio::post(io_context, []() {
        std::cout << "Another one from the queue" << std::endl;
    });

    std::cout << "--- Running io_context ---" << std::endl;

    io_context.run();

    return 0;
}
