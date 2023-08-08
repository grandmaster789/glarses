#ifndef GLARSES_UTIL_ATOMIC_H
#define GLARSES_UTIL_ATOMIC_H

#include <atomic>

namespace glarses {
    class AtomicBool {
    public:
        AtomicBool() = default;

        explicit AtomicBool(      bool               value);
        explicit AtomicBool(const std::atomic<bool>& ab);
                 AtomicBool(const AtomicBool&        ab);

        AtomicBool& operator = (      bool               value);
        AtomicBool& operator = (const std::atomic<bool>& ab);
        AtomicBool& operator = (const AtomicBool&        ab);

        bool operator == (      bool               value) const;
        bool operator == (const std::atomic<bool>& ab)    const;
        bool operator == (const AtomicBool&        ab)    const;

        bool operator != (      bool               value) const;
        bool operator != (const std::atomic<bool>& ab)    const;
        bool operator != (const AtomicBool&        ab)    const;

        bool value() const;

              std::atomic<bool>& get();
        const std::atomic<bool>& get() const;

    private:
        std::atomic<bool> m_Value = false;
    };
}

#endif
