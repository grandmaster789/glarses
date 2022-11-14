#pragma once

#include "dependencies.h"
#include "util/mediator.h"
#include "t5/t5_manager.h"
#include "player.h"

#include <mutex>
#include <vector>

namespace glarses {
	class Player;
	class Application :
		public util::MessageHandler<t5::Manager::GlassesFound>,
		public util::MessageHandler<Player::Quit>
	{
	public:
		Application();
		~Application();

		Application             (const Application&) = delete;
		Application& operator = (const Application&) = delete;
		Application             (Application&&) noexcept = delete;
		Application& operator = (Application&&) noexcept = delete;

		void run();

		void operator()(const t5::Manager::GlassesFound& found);
		void operator()(const Player::Quit& player_quitting);

	private:
		void        init_found_glasses();
		std::string make_player_name();

		std::mutex                m_FoundGlassesMutex;
		std::vector<t5::Glasses*> m_FoundGlasses;

		// NOTE at the moment only one player is actually supported,
		//      although it should be possible at some point to support multiple connections
		//      (both due to tiltfive NDK limitations and the way that the GLFW initialization/shutdown
		//       is currently structured)
		std::vector<std::unique_ptr<Player>> m_Players;
	};
}