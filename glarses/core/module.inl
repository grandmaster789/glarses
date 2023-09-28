#ifndef GLARSES_CORE_MODULE_INL
#define GLARSES_CORE_MODULE_INL

#include "module.h"

namespace glarses::core {
    template <typename T>
    T* ModuleManager::find(const std::string& module_name) {
        return static_cast<T*>(find_base(module_name));
    }
}

#endif
