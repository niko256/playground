#pragma once

#include "error.hpp"
#include "shared_state.hpp"

#include <cassert>
#include <exception>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class Future {
    template <typename U>
    friend class Promise;

  public:
    Future() = default;

    // Non-copyable
    Future(const Future&) = delete;
    Future& operator=(const Future&) = delete;

    // Movable
    Future(Future&&) = default;
    Future& operator=(Future&&) = default;

    // One-shot
    T Get() {
        if (!Valid()) {
            throw NoStateError{};
        }

        auto state = std::move(state_);

        std::unique_lock lock(state->mutex_);
        state->cv_.wait(lock, [&] {
            return state->ready_;
        });

        auto& res = state->result_.value();

        if (res.has_value()) {
            return std::move(res.value());
        } else {
            std::rethrow_exception(res.error());
        }
    }

    bool Valid() const {
        return state_ != nullptr;
    }

  private:
    explicit Future(std::shared_ptr<SharedState<T>> state)
        : state_(std::move(state)) {
    }

  private:
    std::shared_ptr<SharedState<T>> state_{nullptr};
};
