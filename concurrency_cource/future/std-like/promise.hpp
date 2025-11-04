#pragma once

#include "error.hpp"
#include "future.hpp"
#include "shared_state.hpp"

#include <expected>
#include <memory>
#include <stdexcept>

template <typename T>
class Promise {
  public:
    Promise()
        : state_(std::make_shared<SharedState<T>>()) {
    }

    ~Promise() {
        if (state_) {
            std::lock_guard lock(state_->mutex_);
            if (!state_->ready_) {
                state_->result_.emplace(
                    std::unexpected(std::make_exception_ptr(BrokenPromiseError{})));
                SetReadyAndNotifyLocked();
            }
        }
    }

    // Non-copyable
    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;

    // Movable
    Promise(Promise&&) = default;
    Promise& operator=(Promise&&) = default;

    // One-shot
    Future<T> MakeFuture() {
        if (!state_) {
            throw NoStateError{};
        }

        if (state_->future_retrieved_.exchange(true)) {
            throw std::logic_error("Future already retrieved from this Promise");
        }

        return Future<T>{state_};
    }

    // One-shot
    void SetValue(T value) {
        if (!state_) {
            throw NoStateError{};
        }

        std::lock_guard lock(state_->mutex_);
        if (state_->ready_) {
            throw PromiseAlreadySatisfiedError{};
        }
        state_->result_.emplace(std::move(value));
        SetReadyAndNotifyLocked();
    }

    // One-shot
    void SetException(std::exception_ptr e_ptr) {
        if (!state_) {
            throw NoStateError{};
        }

        std::lock_guard lock(state_->mutex_);
        if (state_->ready_) {
            throw PromiseAlreadySatisfiedError{};
        }
        state_->result_.emplace(std::unexpected(std::move(e_ptr)));
        SetReadyAndNotifyLocked();
    }

  private:
    void SetReadyAndNotifyLocked() {
        state_->ready_ = true;
        state_->cv_.notify_one();
    }

  private:
    std::shared_ptr<SharedState<T>> state_;
};
