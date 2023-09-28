#ifndef GLARSES_OS_DYNAMIC_LIBRARY_H
#define GLARSES_OS_DYNAMIC_LIBRARY_H

#include <filesystem>
#include <memory>

namespace glarses::os {
    //
    // a more comprehensive wrapping method would be to use boost.dll
    // but boost has a significant compilation overhead, so we're just
    // going to use our own minimal set
    //
    class DynamicLibrary {
    public:
        explicit DynamicLibrary(const std::filesystem::path& source_path);
        ~DynamicLibrary();

        DynamicLibrary             (const DynamicLibrary&) = delete;
        DynamicLibrary& operator = (const DynamicLibrary&) = delete;

        static std::unique_ptr<DynamicLibrary> locate(
                const std::string&           simple_name,
                const std::filesystem::path& search_directory = ""
        );

        void  load();
        void  unload();
        void* get_handle() const;

        [[nodiscard]]
        bool is_live() const noexcept;

        [[nodiscard]]
        void* get_symbol(const std::string& name) const;

    private:
        bool                  m_Live   = false;
        void*                 m_Handle = nullptr;
        std::filesystem::path m_SourcePath;
    };
}

#endif
