#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

        auto handle_signals = [&](const boost::system::error_code& ec, int signal) {
            if (!ec) {
                std::sout << "Application recieved the signal : " << signal << std::endl;

                // signal recieved => application suspend point
                io_context.stop();
            }

            signals.async_wait(handle_signals);

            std::cout << "Application is running, press CTRL + C to stop and shut it down" << std::endl;

            io_context.run();
        };
    } catch (std::exception& e) {
        std::cerr << "Application recieved an exception : " << e.what() << std::endl;
    }
    return 0;
}
