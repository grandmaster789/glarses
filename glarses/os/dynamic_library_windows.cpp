#include "dynamic_library.h"
#include "../dependencies.h"
#include "../log/logger.h"
#include <cassert>

namespace glarses::os {
    DynamicLibrary::DynamicLibrary(const std::filesystem::path& source_path):
        m_SourcePath(source_path)
    {
    }

    DynamicLibrary::~DynamicLibrary() {
        if (is_live())
            unload();
    }

    std::unique_ptr<DynamicLibrary> DynamicLibrary::locate(
            const std::string&           simple_name,
            const std::filesystem::path& search_directory
    ) {
        auto p = std::filesystem::path(simple_name);

        // if no extension was provided, assume .dll
        if (simple_name.find('.') == std::string::npos)
            p = std::filesystem::path(simple_name + ".dll");

        // if a specific directory was provided, add it; otherwise assume cwd
        if (search_directory != "")
            p = search_directory / p;

        if (std::filesystem::exists(p))
            return std::make_unique<DynamicLibrary>(p);
        else
            g_LogError << "Library does not exist: " << p.generic_string();

        return nullptr;
    }

    void DynamicLibrary::load() {
        assert(!m_Live);
        m_Handle = LoadLibraryEx(m_SourcePath.generic_string().c_str(), nullptr, 0);

        assert(m_Handle);
        m_Live = true;
    }

    void DynamicLibrary::unload() {
        assert(m_Live);
        FreeLibrary(static_cast<HINSTANCE>(m_Handle));
        m_Live = false;
    }

    void* DynamicLibrary::get_handle() const {
        return m_Handle;
    }

    bool DynamicLibrary::is_live() const noexcept {
        return m_Live;
    }

    void* DynamicLibrary::get_symbol(const std::string& name) const {
        assert(is_live());
        return GetProcAddress(static_cast<HINSTANCE>(m_Handle), name.c_str());
    }
}
