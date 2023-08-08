#ifndef GLARSES_UTIL_DOUBLE_STACK_ALLOCATOR_H
#define GLARSES_UTIL_DOUBLE_STACK_ALLOCATOR_H

#include "allocator_base.h"

namespace glarses::util {
    // NOTE - this still allocates on the heap, just using a stack data structure
    //        either end of the stack can be deallocated, but nothing else
    // NOTE - while this is an allocator, specifically this one doesn't actually
    //        implement single allocate/deallocate... this is not a drop-in replacement
    //        and should only be used in very specific situations.
    //        Also the deallocate model is different...
    //        It's pretty questionable to let this inherit from AllocatorBase
    struct DoubleStackAllocator:
            AllocatorBase
    {
        DoubleStackAllocator() = default;
        ~DoubleStackAllocator() override;

        explicit DoubleStackAllocator(size_t total_size);

        DoubleStackAllocator             (const DoubleStackAllocator&) = delete;
        DoubleStackAllocator& operator = (const DoubleStackAllocator&) = delete;
        DoubleStackAllocator             (DoubleStackAllocator&& sa) noexcept;
        DoubleStackAllocator& operator = (DoubleStackAllocator&& sa) noexcept;

        void* allocate(size_t num_bytes, size_t alignment) override;
        void  deallocate(void* pointer) override;

        void* allocate_top   (size_t size, size_t alignment);
        void* allocate_bottom(size_t size, size_t alignment);

        void deallocate_top   (size_t size);
        void deallocate_bottom(size_t size);

        void clear();
        void clear_top();
        void clear_bottom();

        uint8_t* m_Memory     = nullptr;
        size_t   m_TotalBytes = 0;
        size_t   m_Top        = 0;
        size_t   m_Bottom     = 0;
    };
}

#endif
