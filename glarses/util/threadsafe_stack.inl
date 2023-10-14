#ifndef GLARSES_UTIL_THREADSAFE_STACK_INL
#define GLARSES_UTIL_THREADSAFE_STACK_INL

#include "threadsafe_stack.h"
#include <iterator>

namespace glarses {
    template <typename T>
    ThreadSafeStack<T>::ThreadSafeStack(const ThreadSafeStack<T>& tss) {
        std::lock_guard guard1(tss.m_Mutex);

        m_Items = tss.m_Items;
    }

    template <typename T>
    ThreadSafeStack<T>& ThreadSafeStack<T>::operator = (const ThreadSafeStack<T>& tss) {
        std::lock_guard guard1(tss.m_Mutex);
        std::lock_guard guard2(m_Mutex);

        m_Items = tss.m_Items;

        return *this;
    }

    template <typename T>
    ThreadSafeStack<T>::ThreadSafeStack(ThreadSafeStack<T>&& tss) noexcept {
        std::lock_guard guard1(tss.m_Mutex);

        m_Items = std::move(tss.m_Items);
    }

    template <typename T>
    ThreadSafeStack<T>& ThreadSafeStack<T>::operator = (ThreadSafeStack<T>&& tss) noexcept {
        std::lock_guard guard1(tss.m_Mutex);
        std::lock_guard guard2(m_Mutex);

        m_Items = std::move(tss.m_Items);

        return *this;
    }

    template <typename T>
    ThreadSafeStack<T>::ThreadSafeStack(size_t initial_capacity) {
        m_Items.reserve(initial_capacity);
    }

    template <typename T>
    template <typename U>
    void ThreadSafeStack<T>::push(U&& item) {
        std::lock_guard guard(m_Mutex);
        m_Items.push_back(std::forward<U>(item));
    }

    template <typename T>
    template <typename t_Iterator>
    void ThreadSafeStack<T>::push_bulk(t_Iterator first, size_t count) {
        std::lock_guard guard(m_Mutex);
        std::copy_n(first, count, std::back_inserter(m_Items));
    }

    template <typename T>
    bool ThreadSafeStack<T>::try_pop(T& item) {
        std::lock_guard guard(m_Mutex);

        if (m_Items.empty())
            return false;

        item = std::move(m_Items.back());
        m_Items.pop_back();

        return true;
    }

    template <typename T>
    bool ThreadSafeStack<T>::is_empty() const noexcept {
        std::lock_guard guard(m_Mutex);
        return m_Items.empty();
    }
}

#endif
