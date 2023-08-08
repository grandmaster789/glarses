#include "heap_allocator.h"
#include "memory_statistics.h"
#include "tlsf.h"

#include <iostream>

namespace glarses::util {
    HeapAllocator::HeapAllocator(size_t total_size):
        m_TotalBytes(total_size)
    {
        m_Memory = malloc(total_size);
        m_Handle = tlsf_create_with_pool(m_Memory, total_size);

        std::cout << "HeapAllocator created with capacity " << total_size << '\n';
    }

    HeapAllocator::~HeapAllocator() {
        if (m_Handle) {
            MemoryStatistics stats(m_TotalBytes);

            auto pool = tlsf_get_pool(m_Handle);
            tlsf_walk_pool(pool, usage_walker, static_cast<void*>(&pool));
        }
    }

    HeapAllocator::HeapAllocator(HeapAllocator&& ha) noexcept:
        m_Handle        (ha.m_Handle),
        m_Memory        (ha.m_Memory),
        m_AllocatedBytes(ha.m_AllocatedBytes),
        m_TotalBytes    (ha.m_TotalBytes)
    {
        ha.m_Handle         = nullptr;
        ha.m_Memory         = nullptr;
        ha.m_AllocatedBytes = 0;
        ha.m_TotalBytes     = 0;
    }

    HeapAllocator& HeapAllocator::operator = (HeapAllocator&& ha) noexcept {
        std::swap(m_Handle,         ha.m_Handle);
        std::swap(m_Memory,         ha.m_Memory);
        std::swap(m_AllocatedBytes, ha.m_AllocatedBytes);
        std::swap(m_TotalBytes,     ha.m_TotalBytes);

        return *this;
    }

    void* HeapAllocator::allocate(
            size_t num_bytes,
            size_t // alignment
    ) {
        return tlsf_malloc(m_Handle, num_bytes);
    }

    void HeapAllocator::deallocate(void* pointer) {
        tlsf_free(m_Handle, pointer);
    }
}