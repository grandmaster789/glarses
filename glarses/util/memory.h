#ifndef GLARSES_UTIL_MEMORY_H
#define GLARSES_UTIL_MEMORY_H

#include "service.h"

#include "detail/memory_statistics.h"
#include "detail/memory_utils.h"
#include "detail/heap_allocator.h"
#include "detail/stack_allocator.h"
#include "detail/double_stack_allocator.h"
#include "detail/linear_allocator.h"
#include "detail/mallocator.h"

namespace glarses {
    struct MemoryService:
        public Service
    {
        struct Configuration {
            size_t m_Capacity = 0; // (defaults to 32MB)
        };

        static MemoryService* instance();

        void init(void* configuration) override;
        void shutdown() override;

        util::LinearAllocator m_ScratchAllocator;
        util::HeapAllocator   m_SystemAllocator;
    };

    template <typename T>
    [[nodiscard]]
    inline T* alloc(
            size_t               num_elements,
            util::AllocatorBase* allocator
    ) {
        return static_cast<T*>(allocator->allocate(num_elements * sizeof(T), 1));
    }

    template <typename T>
    inline void dealloc(
            T*                   ptr,
            util::AllocatorBase* allocator
    ) {
        allocator->deallocate(ptr);
    }
}

#endif
