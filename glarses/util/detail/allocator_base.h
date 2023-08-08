#ifndef GLARSES_UTIL_ALLOCATOR_BASE_H
#define GLARSES_UTIL_ALLOCATOR_BASE_H

#include <cstdint>

namespace glarses::util {
    // malloc/free-like interface
    // NOTE - the alignment generally doesn't change, probably better as a template
    //        this is *not* an especially good allocator model...
    //        the only good thing is that it's simple
    struct AllocatorBase {
        virtual ~AllocatorBase() = default;

        virtual void* allocate(
                size_t num_bytes,
                size_t alignment
        ) = 0;

        virtual void deallocate(void* pointer) = 0;
    };
}

#endif