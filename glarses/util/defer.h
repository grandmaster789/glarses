#ifndef GLARSES_UTIL_DEFER_H
#define GLARSES_UTIL_DEFER_H

#include <optional>

namespace glarses {
    // Run something upon scope exit
    // template Fn must be callable void() type
    //
    // Simplest way to use this is to provide a lambda, for example:
    //    auto guard = defer([=] { fclose(handle); });
    template <typename Fn>
    class Defer {
    public:
        Defer() = default;
        ~Defer();

        explicit Defer(Fn&& callback);

        Defer             (const Defer&)     = delete;
        Defer& operator = (const Defer&)     = delete;
        Defer             (Defer&& d) noexcept;
        Defer& operator = (Defer&& d) noexcept;

    private:
        std::optional<Fn> m_Callback;
    };

    template <typename Fn>
    [[nodiscard]] Defer<Fn> defer(Fn&& callback);
}

#include "defer.inl"

#endif