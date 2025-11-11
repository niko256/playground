#pragma once

#include "utils.hpp"
#include <algorithm>
#include <function2/function2.hpp>
#include <sure/context.hpp>
#include <sure/stack/mmap.hpp>
#include <sure/trampoline.hpp>
#include <twist/ed/std/atomic.hpp>

namespace exe::fiber {

class Coroutine : private sure::ITrampoline {
  private:
    using Body = fu2::unique_function<void()>;

    Body routine_;
    ExecutionContext callee_context_;
    ExecutionContext caller_context_;
    Stack stack_;
    bool is_done_;

  private:
    /* ITrampoline::Run */
    void Run() noexcept override;

    static Stack AllocateStack(size_t size = 256 * 1024);

  public:
    explicit Coroutine(Body);

    void Resume();
    void Suspend();

    bool IsDone() const;
};

};  // namespace exe::fiber
