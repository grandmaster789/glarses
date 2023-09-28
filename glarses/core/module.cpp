#include "module.h"
#include "../log/logger.h"
#include "../io/search_paths.h"

namespace glarses::core {
    void AbstractModule::on_load() {
    }

    void AbstractModule::on_unload() {
    }

    ModuleInfo::ModuleInfo(glarses::core::ModuleInfo &&mi) noexcept:
        m_Instance(mi.m_Instance),
        m_DLL(std::move(mi.m_DLL))
    {
        mi.m_Instance = nullptr;
    }

    ModuleInfo& ModuleInfo::operator=(glarses::core::ModuleInfo&& mi) noexcept {
        m_Instance = mi.m_Instance;
        m_DLL      = std::move(mi.m_DLL);

        mi.m_Instance = nullptr;

        return *this;
    }

    ModuleManager& ModuleManager::instance() {
        static ModuleManager mm;
        return mm;
    }

    AbstractModule* ModuleManager::find_base(const std::string& module_name) {
        if (auto result = m_LoadedModules[module_name])
            return result->m_Instance;

        if (auto opt_path = search(module_name)) {
            auto dll = std::make_unique<os::DynamicLibrary>(*opt_path);
            dll->load();

            auto fn = reinterpret_cast<create_module_fn>(dll->get_symbol("create_module"));
            if (!fn) {
                g_LogError << "Dynamic library " << module_name << " does not implement 'create_module'";
                return nullptr;
            }

            if (auto result = fn()) {
                ModuleInfo info;

                info.m_Instance = result;
                info.m_DLL = std::move(dll);
                result->on_load();

                m_LoadedModules.assign(module_name, std::move(info));

                return result;
            }

            g_LogError << "Failed to create module " << module_name;
            return nullptr;
        }

        g_LogError << "Cannot find module: " << module_name;

        return nullptr;
    }

    void ModuleManager::unload(const std::string& module_name) {
        if (auto x = m_LoadedModules[module_name]) {
            x->m_Instance->on_unload();
            m_LoadedModules.erase(module_name);
        }
        else
            g_LogWarning << "Cannot unload '" << module_name << "', module not currently loaded";
    }

    void ModuleManager::unload_all() {
        m_LoadedModules.foreach([](const auto&, ModuleInfo& info) {
            info.m_Instance->on_unload();
        });
        m_LoadedModules.clear();
    }

    std::optional<std::filesystem::path> ModuleManager::search(const std::string& module_name) {
        auto search_paths = {
            io::engine_binaries_path(),
            io::engine_plugin_path(),
            io::project_binaries_path(),
            io::project_plugin_path()
        };

        auto cwd = io::working_dir();

        for (const auto& sp: search_paths) {
            try {
                for (const auto& entry: std::filesystem::recursive_directory_iterator(sp)) {
                    if (entry.is_directory())
                        continue;

                    const auto& x = entry.path();

                    auto filename  = x.filename().string();
                    auto extension = x.extension().string();

                    // NOTE to support linux-y and/or mac variants we may need to either
                    //      complicate this test OR write platform specific detectors.
                    //      Technically this is a windows-only method, which is fine for now
                    if (
                            (extension == ".dll") &&
                            (filename == module_name)
                    )
                        return x;
                }
            }
            catch (...) {
                continue;
            }
        }

        return std::nullopt;
    }
}