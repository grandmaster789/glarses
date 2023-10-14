#ifndef GLARSES_UTIL_THREADSAFE_STACK_H
#define GLARSES_UTIL_THREADSAFE_STACK_H

#include <memory>
#include <vector>
#include <mutex>

namespace glarses {
    template <typename T>
    class ThreadSafeStack {
    public:
        ThreadSafeStack() = default;

        ThreadSafeStack             (const ThreadSafeStack& tss);
        ThreadSafeStack& operator = (const ThreadSafeStack& tss);
        ThreadSafeStack             (ThreadSafeStack&& tss) noexcept;
        ThreadSafeStack& operator = (ThreadSafeStack&& tss) noexcept;

        ThreadSafeStack(size_t initial_capacity);

        template <typename U> // ... this should be a concept, something like convertible_to<T>
        void push(U&& item);

        template <typename t_Iterator> // is there a concept for this?
        void push_bulk(t_Iterator first, size_t count);

        bool try_pop(T& item);

        bool is_empty() const noexcept;

    private:
        mutable std::mutex m_Mutex;
        std::vector<T>     m_Items;
    };
}

#include "threadsafe_stack.inl"

#endif
