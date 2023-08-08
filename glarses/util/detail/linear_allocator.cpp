#include "linear_allocator.h"
#include "memory_utils.h"

#include <utility>
#include <cassert>

namespace glarses::util {
    LinearAllocator::~LinearAllocator() {
        delete[] m_Memory;
    }

    LinearAllocator::LinearAllocator(size_t capacity):
        m_TotalBytes(capacity)
    {
        m_Memory = new uint8_t[capacity];
    }

    LinearAllocator::LinearAllocator(LinearAllocator&& la) noexcept:
        m_Memory    (la.m_Memory),
        m_TotalBytes(la.m_TotalBytes),
        m_Allocated (la.m_Allocated)
    {
        la.m_Memory     = nullptr;
        la.m_TotalBytes = 0;
        la.m_Allocated  = 0;
    }

    LinearAllocator& LinearAllocator::operator = (LinearAllocator&& la) noexcept {
        std::swap(la.m_Memory,     m_Memory);
        std::swap(la.m_Allocated,  m_Allocated);
        std::swap(la.m_TotalBytes, m_TotalBytes);

        return *this;
    }

    void* LinearAllocator::allocate(
            size_t size,
            size_t alignment
    ) {
        assert(size > 0);

        size_t address = memory_align(m_Allocated, alignment);

        if (address > m_TotalBytes) {
            assert(false && "Out of memory; allocated buffer is too small");
            return nullptr;
        }

        m_Allocated = address + size;

        return m_Memory + address;
    }

    void LinearAllocator::deallocate(
            void* /*pointer*/
    ) {
        // no-op without warning - because this can be used polymorphically this may be legitimate
    }

    void LinearAllocator::clear() {
        m_Allocated = 0;
    }
}