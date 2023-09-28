#ifndef GLARSES_IO_SEARCH_PATHS_H
#define GLARSES_IO_SEARCH_PATHS_H

#include <filesystem>

namespace glarses::io {
    std::filesystem::path working_dir();
    std::filesystem::path engine_dir();
    std::filesystem::path engine_shader_path();
    std::filesystem::path engine_asset_path();
    std::filesystem::path engine_binaries_path();
    std::filesystem::path engine_plugin_path();

    void                  set_current_project(const std::filesystem::path& src);
    std::filesystem::path current_project();
    std::filesystem::path project_path();
    std::filesystem::path project_shader_path();
    std::filesystem::path project_asset_path();
    std::filesystem::path project_binaries_path();
    std::filesystem::path project_plugin_path();
}

#endif
