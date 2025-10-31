#pragma once

#include <cstddef>
#include <cstdint>
#include <twist/ed/std/atomic.hpp>
#include <twist/ed/wait/futex.hpp>

class WaitGroup {
  public:
    void Add(size_t count) {
        counter_.fetch_add(count);
    }

    void Done() {
        /* Get the new value after decrementing the counter */
        uint32_t new_val = counter_.fetch_sub(1) - 1;

        /*
         * Wake waiting threads ONLY if:
         * 1. There are threads waiting (wait flag is set) AND
         * 2. Counter has reached zero
         * This avoids unnecessary WakeAll calls
         */
        if ((new_val & kWaitFlag) != 0 && (new_val & kCounterMask) == 0) {
            /* Prepare wake key BEFORE clearing the wait flag */
            auto wake_key = twist::ed::futex::PrepareWake(counter_);

            /* Clear the wait flag - atomic operation */
            counter_.fetch_and(kCounterMask);
            /* Wake all threads waiting on this futex */
            twist::ed::futex::WakeAll(wake_key);
        }
    }

    void Wait() {
        uint32_t current = counter_.load();

        /* Loop until counter reaches zero */
        while (current != 0) {
            /*
             * If wait flag is already set OR we successfully set it via CAS,
             * then sleep on the futex
             */
            if ((current & kWaitFlag) != 0 ||
                counter_.compare_exchange_strong(current, current | kWaitFlag)) {
                /* Sleep on futex with current value including wait flag */
                twist::ed::futex::Wait(counter_, current | kWaitFlag);
            }
            /* Reload current value after wakeup (spurious or real) */
            current = counter_.load();
        }
    }

  private:
    /*
     * Bit layout of counter_:
     * [31]      [30-0]
     * WaitFlag  Counter
     *
     * This allows atomic operations on both counter and wait state
     */
    static constexpr uint32_t kWaitFlag = 0x80000000;    /* Bit 31: wait flag */
    static constexpr uint32_t kCounterMask = 0x7FFFFFFF; /* Bits 30-0: counter mask */

    /* Atomic variable storing both counter and wait flag */
    twist::ed::std::atomic<uint32_t> counter_{0};
};
