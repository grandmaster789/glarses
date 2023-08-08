#ifndef GLARSES_UTIL_STACK_ALLOCATOR_H
#define GLARSES_UTIL_STACK_ALLOCATOR_H

#include "allocator_base.h"

namespace glarses::util {
    // NOTE - this still allocates on the heap, just using a stack data structure
    //        as a result, only the last allocation can be deallocated
    struct StackAllocator:
            AllocatorBase
    {
        StackAllocator() = default;
        ~StackAllocator() override;

        explicit StackAllocator(size_t total_size);

        StackAllocator             (const StackAllocator&) = delete;
        StackAllocator& operator = (const StackAllocator&) = delete;
        StackAllocator             (StackAllocator&& sa) noexcept;
        StackAllocator& operator = (StackAllocator&& sa) noexcept;

        void* allocate(size_t num_bytes, size_t alignment) override;
        void  deallocate(void* pointer) override;

        void clear();

        uint8_t* m_Memory         = nullptr;
        size_t   m_TotalBytes     = 0;
        size_t   m_AllocatedBytes = 0;
    };
}

#endif
