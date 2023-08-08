#ifndef GLARSES_UTIL_HEAP_ALLOCATOR_H
#define GLARSES_UTIL_HEAP_ALLOCATOR_H

#include "allocator_base.h"

namespace glarses::util {
    /*
     * Two Level Segregated Fit (TLSF)-based allocator, see dependencies/include/tlsf.h
     *
     * This should be the default heap allocator to be used
     */
    struct HeapAllocator:
            AllocatorBase
    {
        HeapAllocator() = default;
        ~HeapAllocator() override;

        explicit HeapAllocator(size_t total_size);

        HeapAllocator             (const HeapAllocator&) = delete;
        HeapAllocator& operator = (const HeapAllocator&) = delete;
        HeapAllocator             (HeapAllocator&&) noexcept;
        HeapAllocator& operator = (HeapAllocator&&) noexcept;

        void* allocate(size_t num_bytes, size_t alignment) override;
        void  deallocate(void* pointer) override;

        void*  m_Handle         = nullptr; // tlsf
        void*  m_Memory         = nullptr;
        size_t m_AllocatedBytes = 0;
        size_t m_TotalBytes     = 0;
    };
}

#endif