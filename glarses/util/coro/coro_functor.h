    #ifndef GLARSES_UTIL_CORO_FUNCTOR_H
#define GLARSES_UTIL_CORO_FUNCTOR_H

#include <coroutine>
#include <cstdint>

namespace glarses::util::coro {
    class CoroFunctor {
    public:
        // default construction to allow storage in containers
        CoroFunctor() = default;

        CoroFunctor             (const CoroFunctor&) = default;
        CoroFunctor& operator = (const CoroFunctor&) = default;
        CoroFunctor             (CoroFunctor&& cf) noexcept; // destructive move
        CoroFunctor& operator = (CoroFunctor&& cf) noexcept; // destructive move

        // construct from coroutine handles
        template <typename T>
        CoroFunctor(const T& coro) noexcept
        requires(std::is_convertible_v<T, std::coroutine_handle<>>);

        // construct from function objects
        template <typename T>
        CoroFunctor(T* functor) noexcept
        requires(
                !std::is_same_v<std::remove_reference_t<T>, CoroFunctor> &&
                !std::is_convertible_v<T, std::coroutine_handle<>>
        );

        // construct from function object lvalue (makes a copy)
        template <typename T>
        CoroFunctor(const T& functor) noexcept
        requires(
                !std::is_same_v<std::remove_reference_t<T>, CoroFunctor> &&
                !std::is_convertible_v<T, std::coroutine_handle<>> &&
                std::is_copy_constructible_v<T>
        );

        // construct from function object rvalue (moves the parameter)
        template <typename T>
        CoroFunctor(T&& functor) noexcept
        requires(
                !std::is_reference_v<T> &&
                !std::is_same_v<std::remove_reference_t<T>, CoroFunctor> &&
                !std::is_convertible_v<T, std::coroutine_handle<>>
        );

        // construct from free function (non-coroutine)
        CoroFunctor(void(*free_function)()) noexcept;

        void operator()() noexcept; // NOTE exceptions are not allowed!

        [[nodiscard]] bool                    is_coroutine() const noexcept;
        [[nodiscard]] std::coroutine_handle<> as_coroutine()       noexcept; // will brick if the wrapped function is not a coroutine

    private:
        // Pointer tag bit (tag regular, non-coroutine function pointers)
        // NOTE using low bits may also be possible, but consequences are somewhat unclear
        //      needs more research...
        static_assert(sizeof(void*) == 8); // only works with 64-bit pointers
        static constexpr uintptr_t k_RegularFunctionPtrBit = 1ull << 60;

        void* m_Fn     = nullptr;
        void* m_Object = nullptr; // may remain nullptr when referring to a global function
    };
}

#include "coro_functor.inl"

#endif
