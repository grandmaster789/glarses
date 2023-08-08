#include "atomic.h"

namespace glarses {
    AtomicBool::AtomicBool(bool value):
        m_Value(value)
    {
    }

    AtomicBool::AtomicBool(const std::atomic<bool>& ab):
        m_Value(ab.load(std::memory_order_acquire))
    {
    }

    AtomicBool::AtomicBool(const AtomicBool& ab):
        m_Value(ab.m_Value.load(std::memory_order_acquire))
    {
    }

    AtomicBool& AtomicBool::operator = (bool value) {
        m_Value.store(value, std::memory_order_release);
        return *this;
    }

    AtomicBool& AtomicBool::operator = (const std::atomic<bool>& ab) {
        m_Value.store(
                ab.load(std::memory_order_acquire),
                std::memory_order_release
        );

        return *this;
    }

    AtomicBool& AtomicBool::operator = (const AtomicBool& ab) {
        m_Value.store(
                ab.m_Value.load(std::memory_order_acquire),
                std::memory_order_release
        );

        return *this;
    }

    bool AtomicBool::operator == (bool value) const {
        return
            m_Value.load(std::memory_order_acquire) == value;
    }

    bool AtomicBool::operator == (const std::atomic<bool>& ab) const {
        return
            m_Value.load(std::memory_order_acquire) == ab.load(std::memory_order_acquire);
    }

    bool AtomicBool::operator == (const AtomicBool& ab) const {
        return
            m_Value.load(std::memory_order_acquire) == ab.m_Value.load(std::memory_order_acquire);
    }

    bool AtomicBool::operator != (bool value) const {
        return
            m_Value.load(std::memory_order_acquire) != value;
    }

    bool AtomicBool::operator != (const std::atomic<bool>& ab) const {
        return
            m_Value.load(std::memory_order_acquire) != ab.load(std::memory_order_acquire);
    }

    bool AtomicBool::operator != (const AtomicBool& ab) const {
        return
            m_Value.load(std::memory_order_acquire) != ab.m_Value.load(std::memory_order_acquire);
    }

    bool AtomicBool::value() const {
        return
            m_Value.load(std::memory_order_acquire);
    }

    std::atomic<bool>& AtomicBool::get() {
        return m_Value;
    }

    const std::atomic<bool>& AtomicBool::get() const {
        return m_Value;
    }
}