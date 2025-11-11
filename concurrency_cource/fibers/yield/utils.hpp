#pragma once

#include "twist/ed/sure/context.hpp"
#include <cstddef>
#include <twist/ed/sure/stack/mmap.hpp>

namespace exe::fiber {

using Stack = twist::ed::sure::stack::GuardedMmapStack;

using ExecutionContext = twist::ed::sure::ExecutionContext;
}  // namespace exe::fiber
