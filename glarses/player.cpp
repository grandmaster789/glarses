#include "player.h"
#include "util/mediator.h"
#include "util/filesystem.h"

namespace {
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

namespace glarses {
	Player::Player(
		std::string_view name,
		t5::Glasses*     glasses
	):
		m_Name    (name),
		m_Glasses (glasses)
	{
        auto assets = find_assets_folder();

    	m_ShaderProgram = load_shader_sources(
			assets / "shaders" / "simple.vert",
			assets / "shaders" / "simple.frag"
		);

        /*
		m_PerFrameBuffer = UniformBuffer::create<glm::mat4>(0);

		m_Texture = Texture::load_file(assets / "textures" / "debug_color_02.png");*/
		//m_Texture.bind(0);
	}

	void Player::update() {
        if (!m_GlassesInitialized)
			m_GlassesInitialized = m_Glasses->init(m_Name, m_Window.get_handle());

		if (m_GlassesInitialized) {
            auto pos = m_Glasses->get_position();
            auto ori = m_Glasses->get_orientation();

            if (pos && ori)
                std::cout << *pos << ' ' << *ori << '\n';
		}

        // update the monitor window
        m_Window.make_current();
        m_Window.clear();
        m_Window.swap_buffers();

		// notify when the window was closed (even if the glasses didn't fully initialize)
		if (m_Window.should_close())
			util::broadcast(Quit{ this });
	}
}