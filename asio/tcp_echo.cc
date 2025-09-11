#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

constexpr int PORT = 1234;

class Session : public std::enable_shared_from_this<Session> {
  private:
    static const size_t MAX_LENGTH = 1024;

    void do_read() {
        auto self = shared_from_this();

        sock_.async_read_some(boost::asio::buffer(
                                  data_,
                                  MAX_LENGTH),
                              [this, self](boost::system::error_code ec, size_t length) {
                                  if (!ec) {
                                      do_write(length);
                                  }
                              });
    }

    void do_write(std::size_t length) {
        auto self = shared_from_this();

        boost::asio::async_write(sock_, boost::asio::buffer(data_, length), [this, self](boost::system::error_code ec, size_t length) {
            if (!ec) {
                do_read();
            }
        });
    }

    tcp::socket sock_;
    char data_[MAX_LENGTH];

  public:
    Session(tcp::socket sock) : sock_(std::move(sock)) {}

    void start() {
        do_read();
    }

    void stop(boost::asio::io_context io_ctx) {
        io_ctx.stop();
    }
};

class EchoServer {
  private:
    tcp::acceptor acceptor_;

    void do_accept() {
        acceptor_.async_accept([this](
                                   boost::system::error_code ec, tcp::socket sock) {
            if (!ec) {
                std::make_shared<Session>(std::move(sock))->start();
            } else {
                do_accept();
            }
        });
    }

  public:
    EchoServer(boost::asio::io_context& io_context, short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {

        do_accept();
    }
};

int main() {
    try {
        boost::asio::io_context io_context;
        EchoServer server(io_context, PORT);

        io_context.run();
    } catch (std::exception& e) {
        std::cout << "Server got an error: " << e.what() << std::endl;
    }
    return 0;
}
