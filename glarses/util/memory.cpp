#include "memory.h"
#include "tlsf.h"

#include <iostream>

namespace glarses {
    MemoryService* MemoryService::instance() {
        static MemoryService svc; // NOTE this may still be partially initialized...

        return &svc;
    }

    void MemoryService::init(void* configuration) {
        std::cout << "     MemoryService init\n";

        auto* cfg = static_cast<Configuration*>(configuration);
        auto cap = cfg->m_Capacity;

        if (cap == 0)
            cap = 32 * 1024 * 1024 + tlsf_size() + 8; // default to 32mb plus book-keeping

        m_SystemAllocator = util::HeapAllocator(cap);
    }

    void MemoryService::shutdown() {
        std::cout << "     MemoryService shut down\n";
    }
}