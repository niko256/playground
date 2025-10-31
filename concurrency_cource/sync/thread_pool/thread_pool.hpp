#pragma once

#include "queue.hpp"
#include "task.hpp"

#include <twist/ed/std/atomic.hpp>
#include <twist/ed/std/thread.hpp>

#include <cstddef>
#include <vector>

// Fixed-size pool of worker threads

class ThreadPool {
  public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    // Non-copyable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Non-movable
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    void Start();

    void Submit(Task);

    static ThreadPool* Current();

    void Stop();

  private:
    size_t num_threads_;
    std::vector<twist::ed::std::thread> workers_;
    UnboundedBlockingQueue<Task> tasks_;
    twist::ed::std::atomic<bool> stopped_{false};
};
