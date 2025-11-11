#include "exe/runtime/thread_pool.hpp"

#include <cassert>
#include <twist/ed/static/thread_local/ptr.hpp>

namespace exe::runtime {

TWISTED_STATIC_THREAD_LOCAL_PTR(ThreadPool, current_pool);

ThreadPool::ThreadPool(size_t num_threads)
    : num_threads_(num_threads) {
}

void ThreadPool::Start() {
    for (size_t i = 0; i < num_threads_; ++i) {
        workers_.emplace_back([this] {
            current_pool = this;

            while (auto task = tasks_.Pop()) {
                (*task)();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    assert(stopped_.load());
}

void ThreadPool::Submit(Task task) {
    tasks_.Push(std::move(task));
}

ThreadPool* ThreadPool::Current() {
    return current_pool;
}

void ThreadPool::Stop() {
    tasks_.Close();
    for (auto& worker : workers_) {
        worker.join();
    }
    workers_.clear();
    stopped_.store(true);
}
};  // namespace exe::runtime
