#pragma once

#include <thread>
#include <string>

#include "../dependencies.h"
#include "../util/flat_map.h"

namespace t5 {
	class Manager {
	public:
		Manager();
		~Manager();

		Manager             (const Manager&) = delete;
		Manager& operator = (const Manager&) = delete;
		Manager             (Manager&&) noexcept = delete;
		Manager& operator = (Manager&&) noexcept = delete;
		
		struct GlassesConnected {
			std::string        m_ID;
			tiltfive::Glasses* m_Glasses = nullptr; // non-owning
		};

		struct GlassesDisconnected {
			std::string        m_ID;
		};

		struct GlassesReconnected {
			std::string        m_ID;
		};


	private:
		void init_client();
		void loop_poll();
		
		void update_glasses_lookup();

		std::jthread m_Thread;
		bool         m_Exiting = false;

		std::string m_ApplicationID      = "com.grandmaster.glarses";
		std::string m_ApplicationVersion = "0.0.1";

		std::shared_ptr<tiltfive::Client> m_Client;

		util::FlatMap<std::string, std::shared_ptr<tiltfive::Glasses>> m_LookupGlasses;
	};
}