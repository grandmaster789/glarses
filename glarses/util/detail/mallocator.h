#ifndef GLARSES_UTIL_MALLOCATOR_H
#define GLARSES_UTIL_MALLOCATOR_H

#include "allocator_base.h"

namespace glarses::util {
    /*
     * Malloc-free based allocator; with the usual caveats:
     * - deallocations must still be carefully managed (guard against leaks, double-frees)
     * - can get sluggish because of OS-level locking
     * - may result in fragmented memory
     *
     * It is simple though, and relatively universal
     *
     * Preferably this should not be used in the main execution loop;
     * if you need both allocate and free semantics, prefer HeapAllocator
     */
    struct Mallocator:
            AllocatorBase
    {
        void* allocate(size_t size, size_t alignment) override;
        void  deallocate(void* pointer) override;
    };
}

#endif