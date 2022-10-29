#pragma once

#include <thread>
#include <string>

#include "../dependencies.h"

namespace t5 {
	class Manager {
	public:
		Manager();
		~Manager();

		Manager             (const Manager&) = delete;
		Manager& operator = (const Manager&) = delete;
		Manager             (Manager&&) noexcept = delete;
		Manager& operator = (Manager&&) noexcept = delete;
		

	private:
		void init_client();
		void loop_poll();

		std::jthread m_Thread;
		bool         m_Exiting = false;

		std::string m_ApplicationID      = "com.grandmaster.glarses";
		std::string m_ApplicationVersion = "0.0.1";

		std::shared_ptr<tiltfive::Client> m_Client;
	};
}