#include "t5_manager.h"
#include "../util/mediator.h"
#include "../util/algorithm.h"
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
		m_LookupGlasses.clear();

		m_Exiting = true;	
	}

	const std::string& Manager::get_application_id() const {
		return m_ApplicationID;
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

			std::cerr << "Client connection error: " << x.error() << '\n';
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
				else if (x.error() == tiltfive::Error::kNoService) {
					// NOTE for some reason, the first call typically fails with a NoService error...
					std::this_thread::sleep_for(m_ServicePollingInterval);
					continue;
				}
				else {
				 	std::cerr << "Client getServiceVersion error: " << x << '\n';
					break;
				}
			}

			std::cout << "TiltFive Client connected: " << version << '\n';
		}
	}

	void Manager::loop_poll() {
		while (!m_Exiting) {
			update_glasses_list(); // notify additions/removals/re-connection of glasses
			make_connections_exclusive();

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(m_GlassesPollingInterval);
		}

		release_connections();
	}

	void Manager::update_glasses_list() {
		auto wrapped_current_list = m_Client->listGlasses();

		if (wrapped_current_list) {
			auto current_list = *wrapped_current_list;

			// figure out if theres a new one
			const auto& known_keys = m_LookupGlasses.get_keys();

			for (const auto& key : current_list) {
				if (!util::contains(known_keys, key)) {
					for (int i = 0; i < 4; ++i) { // limited amount of retries
						if (auto x = tiltfive::obtainGlasses(key, m_Client)) {
							Glasses* ptr = x->get();
							m_LookupGlasses.assign(key, std::move(*x));							
							util::broadcast(GlassesConnected{ key, ptr });
							break;
						}
						else
							std::cout << "obtainGlasses failed: " << x << '\n';

						std::this_thread::sleep_for(m_GlassesPollingInterval);
					}
				}
			}

			// find out if any of the glasses got disconnected (do we want re-connection logic?)
			std::vector<std::string> disconnected;
			
			for (const auto& glasses : m_LookupGlasses.get_values()) {
				auto x = glasses->getConnectionState();

				if (x) {
					if (*x == ConnectionState::kDisconnected)
						disconnected.push_back(glasses->getIdentifier());
				}
				else
					std::cerr << "Failed to get connection state\n";
			}

			
			for (auto key: disconnected)
				m_LookupGlasses.erase(key);
		}
		else
			std::cerr << "Error listing connected glasses: " << wrapped_current_list.error() << '\n';
	}

	void Manager::make_connections_exclusive() {
		for (const auto& glasses : m_LookupGlasses.get_values()) {
			auto x = glasses->getConnectionState();

			if (x && *x == ConnectionState::kNotExclusivelyConnected) {
				auto ack = glasses->acquire(m_ApplicationID); // -> Reserved state
	
				if (ack)
					std::cout << "Acquired glasses\n";
				else
					std::cerr << "Failed to acquire glasses: " << ack << '\n';
			}

			if (x && *x == ConnectionState::kReserved) {
				auto rdy = glasses->ensureReady(); // -> exclusive state

				if (rdy)
					std::cout << "Exclusive acquired\n";
				else
					std::cerr << "Failed to ensure readyness" << rdy <<  '\n';
			}
		}
	}

	void Manager::release_connections() {
		for (const auto& glasses : m_LookupGlasses.get_values()) {
			auto x = glasses->getConnectionState();

			if (x && *x == ConnectionState::kConnected) {
				auto err = glasses->release();

				if (err)
					std::cout << "Released glasses\n";
				else
					std::cerr << "Failed to release glasses: " << err << '\n';
			}
		}
	}
}