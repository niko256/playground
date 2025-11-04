#pragma once

#include <stdexcept>

struct NoStateError : std::logic_error {
    NoStateError()
        : std::logic_error("Future has no associated state right now!") {
    }
};

struct PromiseAlreadySatisfiedError : std::logic_error {
    PromiseAlreadySatisfiedError()
        : std::logic_error("Promise already satisfied!") {
    }
};

struct BrokenPromiseError : std::logic_error {
    BrokenPromiseError()
        : std::logic_error("Promise is broken!") {
    }
};
