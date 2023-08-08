#ifndef GLARSES_UTIL_MEMORY_STATISTICS_H
#define GLARSES_UTIL_MEMORY_STATISTICS_H

namespace glarses::util {
    /*
     * Keep track of how many allocations were done, and how much memory is in use
     *
     * This is probably most useful for an arena allocation scheme, as other methods
     * will tend to fragment which limit the usefulness of these statistics
     */
    struct MemoryStatistics {
        explicit MemoryStatistics(size_t num_bytes_total);

        void add   (size_t num_bytes);
        void remove(size_t num_bytes);

        size_t m_NumBytesAllocated = 0;
        size_t m_NumBytesTotal     = 0;
        size_t m_NumAllocations    = 0;
    };

    // tlsf walker method
    // NOTE - ptr should be allocated using tlsf, userdata is a MemoryStatistics*
    void usage_walker(void* ptr, size_t size, int used, void* userdata);
}

#endif
