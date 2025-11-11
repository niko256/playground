#include "coroutine.hpp"

namespace exe::fiber {

Coroutine::Coroutine(Body procedure)
    : routine_(std::move(procedure)),
      stack_(Coroutine::AllocateStack()),
      is_done_(false) {
    callee_context_.Setup(stack_.MutView(), this);
}

void Coroutine::Run() noexcept {
    try {
        routine_();
    } catch (...) {
        throw;
    }

    is_done_ = true;
    callee_context_.ExitTo(caller_context_);
}

twist::ed::sure::stack::GuardedMmapStack Coroutine::AllocateStack(size_t size) {
    return twist::ed::sure::stack::GuardedMmapStack::AllocateAtLeastBytes(size);
}

void Coroutine::Resume() {
    caller_context_.SwitchTo(callee_context_);
}

void Coroutine::Suspend() {
    callee_context_.SwitchTo(caller_context_);
}

bool Coroutine::IsDone() const {
    return is_done_;
}

};  // namespace exe::fiber
