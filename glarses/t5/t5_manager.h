#pragma once

#include <thread>
#include <string>

#include "../dependencies.h"
#include "../util/flat_map.h"

namespace t5 {
	class Manager {
	public:
		// Helpers start additional background threads to continuously poll
		using milliseconds    = std::chrono::milliseconds;
		using Client          = tiltfive::Client;
		using Glasses         = tiltfive::Glasses;
		using ConnectionState = tiltfive::ConnectionState;

		Manager();
		~Manager();

		Manager             (const Manager&) = delete;
		Manager& operator = (const Manager&) = delete;
		Manager             (Manager&&) noexcept = delete;
		Manager& operator = (Manager&&) noexcept = delete;
		
		struct GlassesConnected {
			std::string m_ID;
			Glasses*    m_Glasses; // non-owning
		};

		struct GlassesDisconnected {
			std::string m_ID;
		};

		const std::string& get_application_id() const;

	private:
		void init_client();
		void loop_poll();
		
		void update_glasses_list();
		void make_connections_exclusive();
		void release_connections();

		std::jthread m_Thread;
		bool         m_Exiting = false;

		std::string m_ApplicationID      = "com.grandmaster.glarses";
		std::string m_ApplicationVersion = "0.0.1";

		std::shared_ptr<Client> m_Client;

		milliseconds m_ServicePollingInterval = milliseconds(1'000);
		milliseconds m_GlassesPollingInterval = milliseconds(1'000); // how often the connection states are checked

		util::FlatMap<std::string, std::shared_ptr<Glasses>> m_LookupGlasses;
	};
}