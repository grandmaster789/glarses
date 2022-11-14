#include "player.h"
#include "util/mediator.h"

namespace glarses {
	Player::Player(
		const std::string& name,
		t5::Glasses*       glasses
	):
		m_Name    (name),
		m_Glasses (glasses)
	{
		m_GlassesInitialized = m_Glasses->init(name, m_Window.get_handle());
	}

	void Player::update() {
		if (!m_GlassesInitialized)
			m_GlassesInitialized = m_Glasses->init(m_Name, m_Window.get_handle());

		if (m_GlassesInitialized) {
			// update the pose if possible
			if (m_Glasses->is_pose_fresh()) {
				auto pose = m_Glasses->get_pose();

				std::cout
					<< '(' << pose.posGLS_GBD.x
					<< ", " << pose.posGLS_GBD.y
					<< ", " << pose.posGLS_GBD.z
					<< ")\n";
			}

			// update the mirror to the window
			m_Window.make_current();
			m_Window.clear();
			m_Window.swap_buffers();
		}

		// notify when the window was closed (even if the glasses didn't fully initialize)
		if (m_Window.should_close())
			util::broadcast(Quit{ this });
	}
}