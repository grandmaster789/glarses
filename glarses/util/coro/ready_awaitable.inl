#ifndef GLARSES_UTIL_CORO_READY_AWAITABLE_INL
#define GLARSES_UTIL_CORO_READY_AWAITABLE_INL

#include "ready_awaitable.h"

namespace glarses {
    template <typename T>
    ReadyAwaitable<T>::ReadyAwaitable(T value) noexcept:
        m_Value(std::move(value))
    {
    }

    template <typename T>
    bool ReadyAwaitable<T>::await_ready() noexcept {
        return true;
    }

    template <typename T>
    void ReadyAwaitable<T>::await_suspend(std::coroutine_handle<>) noexcept {
    }

    template <typename T>
    T ReadyAwaitable<T>::await_resume() noexcept {
        return std::move(m_Value);
    }

    bool ReadyAwaitable<void>::await_ready() noexcept {
        return true;
    }

    void ReadyAwaitable<void>::await_suspend(std::coroutine_handle<>) noexcept {
    }

    void ReadyAwaitable<void>::await_resume() noexcept {
    }
}

#endif
