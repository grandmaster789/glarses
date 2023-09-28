#ifndef GLARSES_CORE_MODULE_H
#define GLARSES_CORE_MODULE_H

#include <optional>
#include <string>
#include "../util/flat_map.h"
#include "../os/dynamic_library.h"

namespace glarses::core {
    //
    // infrastructure for hot-reloading parts of the code
    // NOTE this doesn't do dependency management (yet); could be cleaner but works fine
    //

    class AbstractModule {
    public:
        virtual ~AbstractModule() = default;

        virtual void on_load();
        virtual void on_unload();

    private:
        AbstractModule() = default; // only instantiate this
    };

    using create_module_fn = AbstractModule*(*)();

    struct ModuleInfo {
        ModuleInfo() = default;
        ~ModuleInfo() = default;

        ModuleInfo             (const ModuleInfo&) = delete;
        ModuleInfo& operator = (const ModuleInfo&) = delete;
        ModuleInfo             (ModuleInfo&& mi) noexcept;
        ModuleInfo& operator = (ModuleInfo&& mi) noexcept;

        AbstractModule*                     m_Instance = nullptr;
        std::unique_ptr<os::DynamicLibrary> m_DLL;
    };

    class ModuleManager {
    public:
        static ModuleManager& instance();

        AbstractModule* find_base(const std::string& module_name); // if the module is not loaded yet, this will attempt to load it

        template <typename T>
        T* find(const std::string& module_name); // typed variant of find_base

        void unload(const std::string& module_name);
        void unload_all();

    private:
        ModuleManager() = default;

        static std::optional<std::filesystem::path> search(const std::string& module_name);

        FlatMap<std::string, ModuleInfo> m_LoadedModules;
    };
}

#include "module.inl"

#endif
