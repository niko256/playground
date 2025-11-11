#include "fiber.hpp"

#include "exe/fiber/core/coroutine.hpp"
#include "exe/fiber/core/scheduler.hpp"
#include "twist/ed/static/thread_local/ptr.hpp"
#include <cstdlib>

namespace exe::fiber {

TWISTED_STATIC_THREAD_LOCAL_PTR(Fiber, current_fiber);

Fiber::Fiber(Scheduler& scheduler, Body routine)
    : coro_(std::move(routine)),
      sched_(scheduler) {
}

Coroutine& Fiber::GetCoro() {
    return this->coro_;
}

Fiber& Fiber::Self() {
    assert(current_fiber != nullptr);
    return *current_fiber;
}

void Fiber::Schedule() {
    sched_.Submit([this] {
        Step();
    });
}

void Fiber::Step() {
    Fiber* prev = current_fiber;

    current_fiber = this;

    coro_.Resume();

    current_fiber = prev;

    // polling
    if (!coro_.IsDone()) {
        this->Schedule();
    } else {
        delete this;
    }
}

[[nodiscard]] Scheduler* Fiber::GetScheduler() const {
    return &sched_;
}

}  // namespace exe::fiber
