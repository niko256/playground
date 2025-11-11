#include "yield.hpp"
#include "../core/fiber.hpp"
#include "exe/runtime/thread_pool.hpp"

namespace exe::fiber {

void Yield() {
    auto& current_fiber = Fiber::Self();
    current_fiber.GetCoro().Suspend();
}

}  // namespace exe::fiber
