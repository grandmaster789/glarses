#ifndef GLARSES_UTIL_META_IDENTITY_H
#define GLARSES_UTIL_META_IDENTITY_H

namespace glarses::util::meta {
    struct Identity {
        template <typename T>
        T operator()(T&& value) const;

        template <typename T, typename U>
        void operator()(T& ref, U&& value);
    };
}

namespace glarses {
    constexpr util::meta::Identity k_Identity{};
}

#include "idenity.inl"

#endif
