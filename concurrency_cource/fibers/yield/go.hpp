#pragma once

#include <exe/fiber/core/body.hpp>
#include <exe/fiber/core/scheduler.hpp>

namespace exe::fiber {

// Considered harmful

void Go(Scheduler&, Body routine);

void Go(Body routine);

}  // namespace exe::fiber
