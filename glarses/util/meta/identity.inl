#ifndef GLARSES_UTIL_META_IDENTITY_INL
#define GLARSES_UTIL_META_IDENTITY_INL

#include "identity.h"
#include <utility>

namespace glarses::util::meta {
    template <typename T>
    T Identity::operator()(T &&value) const {
        return value;
    }

    template <typename T, typename U>
    void Identity::operator()(T& ref, U&& value) {
        ref = std::move(value);
    }
}

#endif
