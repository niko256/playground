#pragma once

#include <cstddef>
#include <cstdint>
#include <twist/ed/std/atomic.hpp>
#include <twist/ed/wait/futex.hpp>

namespace exe::thread {

class WaitGroup {
  public:
    void Add(size_t count) {
        counter_.fetch_add(count);
    }

    void Done() {
        uint32_t new_val = counter_.fetch_sub(1) - 1;

        if ((new_val & kWaitFlag) != 0 && (new_val & kCounterMask) == 0) {
            auto wake_key = twist::ed::futex::PrepareWake(counter_);

            counter_.fetch_and(kCounterMask);
            twist::ed::futex::WakeAll(wake_key);
        }
    }

    void Wait() {
        uint32_t current = counter_.load();

        while (current != 0) {
            if ((current & kWaitFlag) != 0 ||
                counter_.compare_exchange_strong(current, current | kWaitFlag)) {
                twist::ed::futex::Wait(counter_, current | kWaitFlag);
            }
            current = counter_.load();
        }
    }

  private:
    static constexpr uint32_t kWaitFlag = 0x80000000;
    static constexpr uint32_t kCounterMask = 0x7FFFFFFF;

    twist::ed::std::atomic<uint32_t> counter_{0};
};

};  // namespace exe::thread
