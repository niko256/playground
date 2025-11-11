#pragma once

#include <deque>
#include <optional>
#include <twist/ed/std/condition_variable.hpp>
#include <twist/ed/std/mutex.hpp>

namespace exe::runtime {

template <typename T>
class UnboundedBlockingQueue {
  public:
    void Push(T element) {
        std::lock_guard lock(mutex_);
        if (closed_) {
            return;
        }
        queue_.push_back(std::move(element));
        cv_.notify_one();
    }

    std::optional<T> Pop() {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this] {
            return !queue_.empty() || closed_;
        });

        if (!queue_.empty()) {
            T value = std::move(queue_.front());
            queue_.pop_front();
            return value;
        }
        return std::nullopt;
    }

    void Close() {
        std::lock_guard lock(mutex_);
        closed_ = true;
        cv_.notify_all();
    }

  private:
    std::deque<T> queue_;
    twist::ed::std::mutex mutex_;
    twist::ed::std::condition_variable cv_;
    bool closed_{false};
};

};  // namespace exe::runtime
