#pragma once

#include "body.hpp"
#include "coroutine.hpp"
#include "scheduler.hpp"
#include "vvv/list.hpp"

namespace exe::fiber {

// Fiber = Stackful coroutine x Scheduler

class Fiber : public vvv::IntrusiveListNode<Fiber> {
  private:
    Coroutine coro_;
    Scheduler& sched_;

  public:
    Fiber(Scheduler&, Body);

    Coroutine& GetCoro();

    [[nodiscard]] Scheduler* GetScheduler() const;

    void Schedule();

    void Step();

    static Fiber& Self();
};

}  // namespace exe::fiber
