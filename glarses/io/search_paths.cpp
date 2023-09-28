#include "search_paths.h"
#include "../log/logger.h"

namespace {
    std::filesystem::path g_WorkingDir;
    std::filesystem::path g_CurrentProject;
}

namespace glarses::io {
    using namespace std::filesystem;

    path working_dir() {
        if (g_WorkingDir.empty())
            g_WorkingDir = current_path();

        return g_WorkingDir;
    }

    path engine_dir() {
        return working_dir().parent_path();
    }

    path engine_shader_path() {
        return engine_dir() / "shaders";
    }

    path engine_asset_path() {
        return engine_dir() / "assets";
    }

    path engine_binaries_path() {
        return engine_dir() / "bin";
    }

    path engine_plugin_path() {
        return engine_dir() / "plugins";
    }

    void set_current_project(const path& src) {
        g_CurrentProject = src;

        g_Log << "Current project source: " << src.generic_string();
    }

    path current_project() {
        return g_CurrentProject;
    }

    path project_path() {
        return current_project().parent_path();
    }

    path project_shader_path() {
        return project_path() / "shaders";
    }

    path project_asset_path() {
        return project_path() / "assets";
    }

    path project_binaries_path() {
        return project_path() / "bin";
    }

    path project_plugin_path() {
        return project_path() / "plugins";
    }
}