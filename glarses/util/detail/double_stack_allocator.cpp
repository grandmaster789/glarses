#include "double_stack_allocator.h"
#include "memory_utils.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace glarses::util {
    DoubleStackAllocator::DoubleStackAllocator(size_t total_size):
        m_TotalBytes(total_size)
    {
        m_Memory = new uint8_t[total_size];
    }

    DoubleStackAllocator::~DoubleStackAllocator() {
        delete[] m_Memory;
    }

    DoubleStackAllocator::DoubleStackAllocator(DoubleStackAllocator&& da) noexcept:
        m_Memory    (da.m_Memory),
        m_TotalBytes(da.m_TotalBytes),
        m_Top       (da.m_Top),
        m_Bottom    (da.m_Bottom)
    {
    }

    DoubleStackAllocator& DoubleStackAllocator::operator = (DoubleStackAllocator&& da) noexcept {
        std::swap(m_Memory,     da.m_Memory);
        std::swap(m_TotalBytes, da.m_TotalBytes);
        std::swap(m_Top,        da.m_Top);
        std::swap(m_Bottom,     da.m_Bottom);

        return *this;
    }

    void* DoubleStackAllocator::allocate(
            size_t, // sz,
            size_t  //alignment
    ) {
        assert(false);
        return nullptr;
    }

    void DoubleStackAllocator::deallocate(
            void* //pointer
    ) {
        assert(false);
    }

    void* DoubleStackAllocator::allocate_top(
            size_t size,
            size_t alignment
    ) {
        assert(size > 0);

        size_t aligned = memory_align(m_Top - size, alignment);

        if (aligned <= m_Bottom) {
            assert(false && "Allocation crossing bottom boundary");
            return nullptr;
        }

        m_Top = aligned;

        return m_Memory + aligned;
    }

    void* DoubleStackAllocator::allocate_bottom(
            size_t size,
            size_t alignment
    ) {
        assert(size > 0);

        size_t aligned     = memory_align(m_Bottom, alignment);
        size_t new_address = aligned + size;

        if (new_address >= m_Top) {
            assert(false && "Allocation crossing top boundary");
            return nullptr;
        }

        m_Bottom = new_address;

        return m_Memory + new_address;
    }

    void DoubleStackAllocator::deallocate_top(
            size_t size
    ) {
        if (size > m_TotalBytes - m_Top) {
            std::cout << "Warning - deallocating more bytes from the top than available\n";

            m_Top = m_TotalBytes;
        }
        else
            m_Top += size;
    }

    void DoubleStackAllocator::deallocate_bottom(
            size_t size
    ) {
        if (size > m_Bottom) {
            std::cout << "Warning - deallocating more bytes from the bottom than available\n";

            m_Bottom = 0;
        }
        else
            m_Bottom -= size;
    }

    void DoubleStackAllocator::clear() {
        clear_top();
        clear_bottom();
    }

    void DoubleStackAllocator::clear_top() {
        m_Top = m_TotalBytes;
    }

    void DoubleStackAllocator::clear_bottom() {
        m_Bottom = 0;
    }
}
