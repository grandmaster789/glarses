#include "spin_lock.h"
#include <immintrin.h> // this is x64-only

namespace glarses {
    bool SpinLock::try_lock() noexcept {
        return !m_Locked.test_and_set(std::memory_order_acquire);
    }

    void SpinLock::lock() noexcept {
        while (m_Locked.test_and_set(std::memory_order_acquire))
            while (m_Locked.test(std::memory_order_relaxed))
                _mm_pause(); // x64 intrinsic
    }

    void SpinLock::unlock() noexcept {
        m_Locked.clear(std::memory_order_release);
    }

    bool SpinLock::is_locked() const noexcept {
        return m_Locked.test(std::memory_order_acquire);
    }
}
