#ifndef GLARSES_UTIL_CONCEPTS_H
#define GLARSES_UTIL_CONCEPTS_H

#include <concepts>

namespace glarses {
    template <typename T> concept c_IsVoid  =  std::is_void_v<T>;
    template <typename T> concept c_NonVoid = !std::is_void_v<T>;

    // NOTE this is under-specified
    template <typename T>
    concept TypeErasable = requires (T obj) {
        obj.type_erased();
    };
}

#endif
