#include "player.h"
#include "util/mediator.h"
#include "util/filesystem.h"



namespace glarses {
	Player::Player(
		std::string_view name,
		t5::Glasses*     glasses
	):
		m_Name    (name),
		m_Glasses (glasses)
	{
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
                // NOTE - this will say '(-431602080, -431602080, -431602080) q(-431602080, -431602080 ,-431602080, -431602080)'
                //        if there's no t5 board found... -431602080 is the 'INVALID' value for vector/quaternion values
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