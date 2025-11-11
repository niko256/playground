#include "go.hpp"
#include "exe/fiber/core/fiber.hpp"
#include "exe/fiber/core/scheduler.hpp"
#include "twist/ed/static/thread_local/ptr.hpp"

namespace exe::fiber {

void Go(Scheduler& sched, Body routine) {
    auto* newbie = new Fiber(sched, std::move(routine));
    newbie->Schedule();
}

void Go(Body routine) {
    Go(*Fiber::Self().GetScheduler(), std::move(routine));
}

};  // namespace exe::fiber
