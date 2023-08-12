#ifndef GLARSES_UTIL_FUNCTION_INL
#define GLARSES_UTIL_FUNCTION_INL

#include "function.h"

namespace glarses {
    template <typename R, typename...Args>
    template <typename U>
    Function<R(Args...)>::Function(U&& callable) noexcept:
        m_Storage(new Implementation<U>(std::forward<U>(callable)))
    {
    }

    template <typename R, typename...Args>
    Function<R(Args...)>::Function(const Function& fn):
        m_Storage(fn.m_Storage->clone())
    {
    }

    template <typename R, typename...Args>
    Function<R(Args...)>& Function<R(Args...)>::operator = (const Function& fn) {
        m_Storage.reset(fn.m_Storage->clone());
        return *this;
    }

    template <typename R, typename...Args>
    Function<R(Args...)>::Function(Function&& fn) noexcept:
        m_Storage(std::move(fn.m_Storage))
    {
    }

    template <typename R, typename...Args>
    Function<R(Args...)>& Function<R(Args...)>::operator = (Function&& fn) noexcept {
        m_Storage = std::move(fn.m_Storage);
        return *this;
    }

    template <typename R, typename...Args>
    R Function<R(Args...)>::operator()(Args&&... args) const {
        return m_Storage->call(std::forward<Args>(args)...);
    }

    template <typename R, typename...Args>
    template <typename U>
    Function<R(Args...)>::Implementation<U>::Implementation(U &&callable):
        m_StoredFunction(std::forward<U>(callable))
    {
    }

    template <typename R, typename...Args>
    template <typename U>
    Function<R(Args...)>::Interface* Function<R(Args...)>::Implementation<U>::clone() const {
        return new Implementation(*this);
    }

    template <typename R, typename...Args>
    template <typename U>
    R Function<R(Args...)>::Implementation<U>::call(Args&&... args) const {
        return std::invoke(
                m_StoredFunction,
                std::forward<Args>(args)...
        );
    }
}

#endif
