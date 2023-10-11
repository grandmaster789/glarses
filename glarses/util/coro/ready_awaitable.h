#ifndef GLARSES_UTIL_CORO_READY_AWAITABLE_H
#define GLARSES_UTIL_CORO_READY_AWAITABLE_H

#include <coroutine>

namespace glarses {
    // trivial awaitable - wraps a value that is returned as the resume result.
    //                     Suspension is no-op
    template <typename T>
    class ReadyAwaitable {
    public:
        explicit ReadyAwaitable(T value) noexcept;

        bool await_ready()                          noexcept;
        void await_suspend(std::coroutine_handle<>) noexcept;
        T    await_resume()                         noexcept;

    private:
        T m_Value;
    };

    template <>
    class ReadyAwaitable<void> {
    public:
        ReadyAwaitable() noexcept = default;

        bool await_ready()                          noexcept;
        void await_suspend(std::coroutine_handle<>) noexcept;
        void await_resume()                         noexcept;
    };
}

#include "ready_awaitable.inl"

#endif
