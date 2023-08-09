#ifndef GLARSES_UTIL_MOVE_ON_COPY_INL
#define GLARSES_UTIL_MOVE_ON_COPY_INL

#include "move_on_copy.h"
#include <utility>

namespace glarses {
    template <typename T>
    MoveOnCopy<T>::MoveOnCopy(T&& object) noexcept:
        m_Object(std::forward<T>(object))
    {
    }

    template <typename T>
    MoveOnCopy<T>::MoveOnCopy(const MoveOnCopy<T> &moc):
        m_Object(std::move(moc.m_Object))
    {
    }

    template <typename T>
    MoveOnCopy<T>& MoveOnCopy<T>::operator = (const MoveOnCopy<T>& moc) {
        m_Object = std::move(moc.m_Object);
        return *this;
    }

    template <typename T>
    MoveOnCopy<T>::MoveOnCopy(MoveOnCopy<T>&& moc) noexcept:
        m_Object(std::move(moc.m_Object))
    {
    }

    template <typename T>
    MoveOnCopy<T>& MoveOnCopy<T>::operator = (MoveOnCopy<T>&& moc) noexcept {
        m_Object = std::move(moc.m_Object);
        return *this;
    }

    template <typename T>
    void MoveOnCopy<T>::operator()() {
        m_Object();
    }

    template <typename T>
    T& MoveOnCopy<T>::get() {
        return m_Object;
    }

    template <typename T>
    const T& MoveOnCopy<T>::get() const {
        return m_Object;
    }

    template <typename T>
    T MoveOnCopy<T>::release() {
        return std::move(m_Object);
    }
}

#endif