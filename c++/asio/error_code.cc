#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <iostream>

void on_timeout(const boost::system::error_code &ec) {
  if (!ec) {
    std::cout << "Time expired!" << std::endl;
  } else {
    std::cerr << "Error: " << ec.message() << std::endl;
  }
}

int main() {
  boost::asio::io_context io_context_;
  boost::asio::steady_timer timer(io_context_, std::chrono::seconds(3));

  timer.async_wait(&on_timeout);

  io_context_.run();
  return 0;
}
