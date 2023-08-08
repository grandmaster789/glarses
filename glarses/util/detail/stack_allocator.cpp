#include "stack_allocator.h"
#include "memory_utils.h"

#include <utility>
#include <iostream>
#include <cassert>

namespace glarses::util {
    StackAllocator::StackAllocator(size_t total_size):
        m_TotalBytes(total_size)
    {
        m_Memory = new uint8_t[total_size];
    }

    StackAllocator::~StackAllocator() {
        // NOTE - do we want to be notified when a stack allocator is destroyed while it still has live allocations?
        //        ... probably not

        delete[] m_Memory;
    }

    StackAllocator::StackAllocator(StackAllocator&& sa) noexcept:
        m_Memory        (sa.m_Memory),
        m_TotalBytes    (sa.m_TotalBytes),
        m_AllocatedBytes(sa.m_AllocatedBytes)
    {
        sa.m_Memory         = nullptr;
        sa.m_TotalBytes     = 0;
        sa.m_AllocatedBytes = 0;
    }

    StackAllocator& StackAllocator::operator = (StackAllocator&& sa) noexcept {
        std::swap(m_Memory,         sa.m_Memory);
        std::swap(m_TotalBytes,     sa.m_TotalBytes);
        std::swap(m_AllocatedBytes, sa.m_AllocatedBytes);

        return *this;
    }

    void* StackAllocator::allocate(
            size_t num_bytes,
            size_t alignment
    ) {
        size_t aligned        = memory_align(m_AllocatedBytes, alignment);
        size_t next_allocated = aligned + num_bytes;

        if (next_allocated > m_TotalBytes) {
            assert(false);
            std::cerr << "Out of memory\n";
            return nullptr;
        }

        return m_Memory + aligned;
    }

    void StackAllocator::deallocate(void* pointer) {
        if ((pointer < m_Memory) || (pointer >= m_Memory + m_TotalBytes)) {
            std::cerr << "Cannot deallocate pointer outside of memory area\n";
            return;
        }

        if ((pointer >= m_Memory + m_AllocatedBytes)) {
            std::cerr << "Pointer resolves to uninitialized memory, cannot deallocate\n";
            return;
        }

        m_AllocatedBytes = static_cast<uint8_t*>(pointer) - m_Memory;
    }

    void StackAllocator::clear() {
        m_AllocatedBytes = 0;
    }
}