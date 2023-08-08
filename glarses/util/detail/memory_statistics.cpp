#include "memory_statistics.h"
#include <iostream>
#include <format>

namespace glarses::util {
    MemoryStatistics::MemoryStatistics(size_t num_bytes_total):
        m_NumBytesTotal(num_bytes_total)
    {
    }

    void MemoryStatistics::add(size_t num_bytes) {
        m_NumBytesAllocated += num_bytes;
        ++m_NumAllocations;

        if (m_NumBytesAllocated > m_NumBytesTotal)
            std::cerr << "Total number of bytes available was exceeded\n";
    }

    void MemoryStatistics::remove(size_t num_bytes) {
        m_NumBytesAllocated -= num_bytes;
        --m_NumAllocations;
    }

    void usage_walker(
            void*  ptr,
            size_t size,
            int    used,
            void*  userdata
    ) {
        auto* stats = static_cast<MemoryStatistics*>(userdata);

        if (used) {
            stats->add(size);
            std::cout << std::format("Active allocation: {}, {}\n", ptr, size);
        }
    }
}