#ifndef GLARSES_UTIL_SPINLOCK_H
#define GLARSES_UTIL_SPINLOCK_H

#include <atomic>

namespace glarses {
    class SpinLock {
    public:
        SpinLock() = default;

        SpinLock             (const SpinLock&) = delete;
        SpinLock& operator = (const SpinLock&) = delete;
        SpinLock             (SpinLock&&) noexcept = delete;
        SpinLock& operator = (SpinLock&&) noexcept = delete;

        bool try_lock() noexcept;
        void lock()     noexcept; // which... spins
        void unlock()   noexcept;

        bool is_locked() const noexcept;

    private:
        std::atomic_flag m_Locked = ATOMIC_FLAG_INIT;
    };
}

#endif
