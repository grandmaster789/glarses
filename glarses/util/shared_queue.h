#ifndef GLARSES_UTIL_SHARED_QUEUE_H
#define GLARSES_UTIL_SHARED_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace glarses {
    // MPMC thread-safe FIFO queue
    // not particularly optimized, but should be fine for most use cases

    template <typename T>
    class SharedQueue {
    public:
        SharedQueue() = default;

        SharedQueue             (const SharedQueue&) = delete;
        SharedQueue& operator = (const SharedQueue&) = delete;
        SharedQueue             (SharedQueue&&) noexcept = default;
        SharedQueue& operator = (SharedQueue&&) noexcept = default;

        void push(const T& value);
        void push(T&& value) noexcept;

        bool try_pop(T& popped);
        void wait_pop(T& popped);

        bool   is_empty() const;
        size_t size()     const;

    private:
        std::queue<T>           m_Queue;
        mutable std::mutex      m_Mutex;
        std::condition_variable m_Condition;
    };
}

#include "shared_queue.inl"

#endif
