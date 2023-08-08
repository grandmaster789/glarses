#include "mallocator.h"

#include <cstdlib>

namespace glarses::util {
    void* Mallocator::allocate(
            size_t size,
            size_t /* alignment */
    ) {
        return malloc(size);
    }

    void Mallocator::deallocate(
            void* pointer
    ) {
        free(pointer);
    }
}