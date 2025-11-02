#include "coroutine.hpp"

Coroutine::Coroutine(Body procedure)
    : procedure_(std::move(procedure)),
      stack_(Coroutine::AllocateStack()),
      is_done_(false) {
    callee_context_.Setup(stack_.MutView(), this);
}

void Coroutine::Run() noexcept {
    SuspendContext suspend_ctx(this);

    try {
        procedure_(std::move(suspend_ctx));

    } catch (...) {
        throw;
    }

    is_done_ = true;
    callee_context_.ExitTo(caller_context_);
}

sure::stack::GuardedMmapStack Coroutine::AllocateStack(size_t size) {
    return sure::stack::GuardedMmapStack::AllocateAtLeastBytes(size);
}

void Coroutine::Resume() {
    caller_context_.SwitchTo(callee_context_);
}

void Coroutine::Suspend() {
    callee_context_.SwitchTo(caller_context_);
}

void Coroutine::SuspendContext::Suspend() {
    self_->Suspend();
}

bool Coroutine::IsDone() const {
    return is_done_;
}
