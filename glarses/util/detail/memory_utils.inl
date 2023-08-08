#ifndef GLARSES_UTIL_MEMORY_UTILS_INL
#define GLARSES_UTIL_MEMORY_UTILS_INL

#include "memory_utils.h"

namespace glarses::util {
    constexpr size_t memory_align(size_t size, size_t alignment) {
        size_t mask = alignment - 1;
        return (size + mask) & ~mask;
    }
}

#endif
