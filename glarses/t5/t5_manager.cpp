#include "t5_manager.h"
#include "../util/mediator.h"
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
			update_glasses_lookup(); // notify additions/removals/re-connection of glasses

			// 

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
		}
	}

	void Manager::update_glasses_lookup() {
		auto wrapped_current_list = m_Client->listGlasses();

		if (wrapped_current_list) {
			auto current_list = *wrapped_current_list;

			// check known glasses against the list, notify removals
			auto known_ids = m_LookupGlasses.get_keys();

			for (const auto& id : known_ids) {
				if (!util::contains(current_list, id)) {
					// if it wasn't in a disconnected state yet, do so now and notify
					if (m_LookupGlasses[id]->get()) {
						m_LookupGlasses[id]->reset();
						std::cout << "Glasses disconnected: " << id << '\n';
						util::broadcast(GlassesDisconnected{ id });
					}
				}
			}

			// check for new glasses / reconnections
			for (const auto& current_id : current_list) {
				if (m_LookupGlasses.contains(current_id)) {
					if (!*m_LookupGlasses[current_id]) {
						// reconnection case
						auto wrapped_glasses = tiltfive::obtainGlasses(current_id, m_Client);

						if (wrapped_glasses) {
							m_LookupGlasses.assign(current_id, *wrapped_glasses);
							std::cout << "Glasses reconnected: " << current_id << '\n';
							util::broadcast(GlassesReconnected{ current_id });
						}
						else
							std::cerr << "Reconnection failure: " << wrapped_glasses.error() << '\n';
					}					
				}
				else {
					// not known yet -- fresh pair
					auto wrapped_glasses = tiltfive::obtainGlasses(current_id, m_Client);
					
					if (wrapped_glasses) {
						m_LookupGlasses.assign(current_id, *wrapped_glasses);
						std::cout << "Glasses connected: " << current_id << '\n';
						util::broadcast(GlassesConnected{ current_id, wrapped_glasses->get() });
					}
					else
						std::cerr << "Glasses connection failure: " << wrapped_glasses.error() << '\n';
				}
			}
		}
		else
			std::cerr << "Error listing connected glasses: " << wrapped_current_list.error() << '\n';
	}
}