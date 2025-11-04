#pragma once

#include <exception>
#include <expected>
#include <optional>
#include <twist/ed/std/atomic.hpp>
#include <twist/ed/std/condition_variable.hpp>
#include <twist/ed/std/mutex.hpp>

template <typename T>
struct SharedState {
    twist::ed::std::mutex mutex_;
    twist::ed::std::condition_variable cv_;
    bool ready_{false};

    twist::ed::std::atomic<bool> future_retrieved_{false};

    std::optional<std::expected<T, std::exception_ptr>> result_;
};
