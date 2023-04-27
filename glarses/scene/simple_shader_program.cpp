#include "simple_shader_program.h"
#include "../util/filesystem.h"

namespace {
    // TODO this stuff should go somewhere else...

    namespace fs = std::filesystem;
    namespace gl = glarses::opengl;

    fs::path find_assets_folder() {
        fs::path p = fs::current_path();

        while (!(fs::exists(p / "assets"))) {
            if (p.parent_path() == p)
                throw std::runtime_error("Failed to find assets folder");

            p = p.parent_path();
        }

        return p / "assets";
    }

    gl::ShaderProgram load_shader_sources(
        const fs::path& vertex_shader_file,
        const fs::path& fragment_shader_file
    )  {
        auto vtx_source  = glarses::util::read_text_file(vertex_shader_file);
        auto frag_source = glarses::util::read_text_file(fragment_shader_file);

        gl::ShaderProgram result;

        result.compile(vtx_source, frag_source);

        if (!result.is_ready()) {
            std::cerr << "Failed to create shader program: "<< result.get_last_error() << '\n';

            throw std::runtime_error("Failed to create shader program");
        }

        return result;
    }
}

namespace glarses::scene {
    SimpleShaderProgram& SimpleShaderProgram::instance() {
        static SimpleShaderProgram singleton;

        if (singleton.get_handle() == 0)
            singleton.init();

        return singleton;
    }

    void SimpleShaderProgram::init() {
        auto assets = find_assets_folder();

        m_Handle = load_shader_sources(
            assets / "shaders" / "simple.vert",
            assets / "shaders" / "simple.frag"
        ).get_handle();
    }
}