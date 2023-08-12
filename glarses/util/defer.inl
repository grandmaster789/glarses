#ifndef GLARSES_UTIL_DEFER_INL
#define GLARSES_UTIL_DEFER_INL

#include "defer.h"

namespace glarses {
    template<typename T>
    Defer<T>::~Defer() {
        if (m_Callback)
            (*m_Callback)();
    }

    template<typename T>
    Defer<T>::Defer(T &&callback):
            m_Callback(std::forward<T>(callback)) {
    }

    template<typename T>
    Defer<T>::Defer(Defer<T> &&d) noexcept:
            m_Callback(std::move(d.m_Callback)) {
        d.m_Callback = std::nullopt;
    }

    template<typename T>
    Defer<T> &Defer<T>::operator=(Defer &&d) noexcept {
        m_Callback = std::move(d.m_Callback);
        d.m_Callback = std::nullopt;

        return *this;
    }

    template <typename T>
    Defer<T> defer(T&& callback) {
        return Defer(std::forward<T>(callback));
    }
}

#endif
