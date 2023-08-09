#ifndef GLARSES_UTIL_DEFER_H
#define GLARSES_UTIL_DEFER_H

#include <optional>

namespace glarses {
    // Fn must be callable void() type
    //
    template <typename Fn>
    class Defer {
    public:
        Defer() = delete;
        ~Defer() {
            if (m_Callback)
                (*m_Callback)();
        }

        explicit Defer(Fn&& callback):
            m_Callback(std::forward<Fn>(callback))
        {
        }

        Defer             (const Defer&)     = delete;
        Defer& operator = (const Defer&)     = delete;
        Defer             (Defer&&) noexcept = delete;
        Defer& operator = (Defer&&) noexcept = delete;

    private:
        std::optional<Fn> m_Callback;
    };

    template <typename Fn>
    [[nodiscard]] Defer<Fn> defer(Fn&& callback) {
        return Defer(std::forward<Fn>(callback));
    }
}

#endif