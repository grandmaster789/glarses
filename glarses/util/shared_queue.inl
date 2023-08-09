#ifndef GLARSES_UTIL_SHARED_QUEUE_INL
#define GLARSES_UTIL_SHARED_QUEUE_INL

#include "shared_queue.h"

namespace glarses {
    template <typename T>
    void SharedQueue<T>::push(const T& value) {
        {
            std::lock_guard guard(m_Mutex);
            m_Queue.push(value);
        }

        m_Condition.notify_one();
    }

    template <typename T>
    void SharedQueue<T>::push(T&& value) noexcept {
        {
            std::lock_guard guard(m_Mutex);
            m_Queue.push(std::forward<T>(value));
        }

        m_Condition.notify_one();
    }

    template <typename T>
    bool SharedQueue<T>::try_pop(T& popped) {
        std::lock_guard guard(m_Mutex);

        if (m_Queue.empty())
            return false;

        popped = std::move(m_Queue.front());
        m_Queue.pop();

        return true;
    }

    template <typename T>
    void SharedQueue<T>::wait_pop(T& popped) {
        std::unique_lock lock(m_Mutex);

        while (m_Queue.empty())
            m_Condition.wait(lock);

        popped = std::move(m_Queue.front());
        m_Queue.pop();
    }

    template <typename T>
    bool SharedQueue<T>::is_empty() const {
        std::lock_guard guard(m_Mutex);
        return m_Queue.empty();
    }

    template <typename T>
    size_t SharedQueue<T>::size() const {
        std::lock_guard guard(m_Mutex);
        return m_Queue.size();
    }
}

#endif
