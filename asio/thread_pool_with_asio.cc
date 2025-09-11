#include "../../DataStructures/Concurrency/ThreadPool/ThreadPool.hpp"
#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>
#include <thread>

int main() {
    boost::asio::io_context io_;
    auto work = boost::asio::make_work_guard(io_);

    boost::system::error_code ec_;
    data_structures::concurrency::ThreadPool pool_(4);

    pool_.start();

    for (int i = 0; i < 4; ++i) {
        pool_.submit([&io_] { io_.run(); });
    }

    for (int task_id = 0; task_id < 100; ++task_id) {
        boost::asio::post(io_,
                          [] { std::cout << "New task created!" << std::endl; });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (!ec_) {
        std::cout << "All tasks are going to finish..." << std::endl;
        work.reset();
        pool_.stop();
    } else {
        std::cerr << "Something went wrong..." << std::endl;
    }

    return 0;
}
