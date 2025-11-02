#pragma once

#include <algorithm>
#include <function2/function2.hpp>
#include <sure/context.hpp>
#include <sure/stack/mmap.hpp>
#include <sure/trampoline.hpp>
#include <twist/ed/std/atomic.hpp>

class Coroutine : private sure::ITrampoline {
  public:
    class SuspendContext {
        friend class Coroutine;

      private:
        Coroutine* self_;

        explicit SuspendContext(Coroutine* coro)
            : self_(coro) {
        }

      public:
        void Suspend();
    };

  private:
    using Body = fu2::unique_function<void(SuspendContext)>;

    Body procedure_;
    sure::ExecutionContext callee_context_;
    sure::ExecutionContext caller_context_;
    sure::stack::GuardedMmapStack stack_;
    bool is_done_;

  private:
    /* ITrampoline::Run */
    void Run() noexcept override;

    static sure::stack::GuardedMmapStack AllocateStack(size_t size = 4 * 1024 *
                                                                     1024);

  public:
    explicit Coroutine(Body);

    void Resume();
    void Suspend();

    bool IsDone() const;
};
