#include "t5_manager.h"

#include <chrono>

namespace t5 {
	Manager::Manager():
		m_Thread([&] { 
			init_client();

			if (m_Client)
				loop_poll(); 
		})
	{
	}

	Manager::~Manager() {
		m_Exiting = true;
	}

	void Manager::init_client() {
		using namespace std::chrono_literals;

		while (true) {
			auto x = tiltfive::obtainClient(
				m_ApplicationID,
				m_ApplicationVersion,
				nullptr
			);

			if (x) {
				m_Client = std::move(*x);
				break;
			}

			if (x.error().value() == T5_TIMEOUT) {
				std::cout << ".";
				std::this_thread::sleep_for(100ms);
				continue;
			}

			std::cerr << "Client connection error: " <<  x.error() << '\n';
			break;
		}

		if (m_Client) {
			std::string version;

			while (true) {
				auto x = m_Client->getServiceVersion();
				
				if (x) {
					version = std::move(*x);
					break;
				}			

				std::this_thread::sleep_for(100ms);
			}

			std::cout << "TiltFive Client connected: " << version << '\n';
		}
	}

	void Manager::loop_poll() {
		while (!m_Exiting) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
		}
	}
}