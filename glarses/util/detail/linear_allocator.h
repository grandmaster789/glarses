#ifndef GLARSES_UTIL_LINEAR_ALLOCATOR_H
#define GLARSES_UTIL_LINEAR_ALLOCATOR_H

#include "allocator_base.h"

namespace glarses::util {
    /*
     * This is an arena-style allocator, which only allows allocation and does nothing on deallocation
     */
    struct LinearAllocator:
            AllocatorBase
    {
        LinearAllocator() = default;
        ~LinearAllocator() override;

        explicit LinearAllocator(size_t capacity);

        LinearAllocator             (const LinearAllocator&) = delete;
        LinearAllocator& operator = (const LinearAllocator&) = delete;
        LinearAllocator             (LinearAllocator&& la) noexcept;
        LinearAllocator& operator = (LinearAllocator&& la) noexcept;

        void* allocate(size_t size, size_t alignment) override;
        void  deallocate(void* pointer) override;

        void clear();

        uint8_t* m_Memory     = nullptr;
        size_t   m_TotalBytes = 0;
        size_t   m_Allocated  = 0;
    };
}

#endif
