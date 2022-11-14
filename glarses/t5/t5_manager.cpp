#include "t5_manager.h"
#include "t5_glasses.h"
#include "../util/mediator.h"
#include "../util/algorithm.h"

#include <chrono>
#include <string_view>

namespace glarses::t5 {
	Manager::Manager() :
		m_Thread([&] { 
			m_Exiting = !init_client(); // if init failed, skip over the loop entirely
			
			while (!m_Exiting) {
				update_glasses_list(); // this will notify any newly found and/or lost connections 

				for (auto& glasses : m_Glasses.get_values())
					glasses->poll();

				std::this_thread::sleep_for(k_PollingRate);
			}

			t5DestroyContext(&m_Context);
		})
	{
	}
	

	Manager::~Manager() {
		m_Exiting = true;
	}

	Manager& Manager::instance() {
		static Manager x;
		return x;
	}

	void Manager::destroy() {
		m_Exiting = true;
		if (m_Thread.joinable())
			m_Thread.join();
	}

	const std::string& Manager::get_application_id() const {
		return m_ApplicationID;
	}

	const std::string& Manager::get_service_version() const {
		std::scoped_lock guard(m_Mutex);
		return m_ServiceVersion;
	}

	T5_Context Manager::get_context() const {
		return m_Context;
	}

	bool Manager::init_client() {
		using namespace std::chrono_literals;

		T5_ClientInfo info;
		info.applicationId      = m_ApplicationID.c_str();
		info.applicationVersion = m_ApplicationVersion.c_str();

		if (auto err = t5CreateContext(&m_Context, &info, nullptr)) {
			std::cerr << "t5CreateContext: " << t5GetResultMessage(err) << '\n';
			return false;
		}

		// query the services version
		{
			char version_buffer[32] = {};
			auto sz = sizeof(version_buffer);

			for (int i = 0; i < 5; ++i) { // allow a couple of retries
				if (auto err = t5GetSystemUtf8Param(m_Context, kT5_ParamSys_UTF8_Service_Version, version_buffer, &sz)) {
					if (err == T5_ERROR_NO_SERVICE) {
						using namespace std::chrono_literals;
						std::this_thread::sleep_for(100ms);
						continue;
					}

					std::cerr << "t5GetSystemUtf8Param: " << t5GetResultMessage(err) << '\n';
					return false;
				}
				else {
					instance().set_service_version(std::string(version_buffer, sz));
					break;
				}
			}
		}

		return true;
	}

	void Manager::update_glasses_list() {
		std::vector<char> ids(64);

		// allow a single retry to allow for expanding the buffer
		for (int i = 0; i < 2; ++i) {
			size_t sz = ids.size();

			if (auto err = t5ListGlasses(m_Context, ids.data(), &sz)) {
				if (err == T5_ERROR_NO_SERVICE)
					return; // should just retry in the next poll

				if (err == T5_ERROR_OVERFLOW) {
					// valid retry
					ids.resize(sz);
					continue;
				}

				// all other error types should just be reported
				std::cerr << "t5ListGlasses: " << t5GetResultMessage(err) << '\n';
				return;
			}
			else
				break;
		}

		// split the ids on null terminators
		std::vector<std::string> split_list;
		
		{
			const auto* ptr = ids.data();

			while (true) {
				std::string x(ptr);  // will initialize up to the first '\0'
				ptr += x.size() + 1; // advance pointer beyond 

				if (x.empty())
					break;

				split_list.push_back(std::move(x));
			}
		}

		// compare against known glasses
		auto known_ids = m_Glasses.get_keys();
		auto new_ids   = util::set_difference(split_list, known_ids);
		auto lost_ids  = util::set_difference(known_ids, split_list);

		// notify newly found glasses
		for (auto id : new_ids) {
			auto obj = std::make_unique<Glasses>(id);
			auto* weak = obj.get();

			m_Glasses.assign(id, std::move(obj));

			util::broadcast(GlassesFound{ id, weak });
		}

		// notify lost glasses
		for (auto id : lost_ids) {
			auto* weak = m_Glasses[id]->get();
			util::broadcast(GlassesLost{ id, weak });
			m_Glasses.erase(id);
		}
	}

	void Manager::set_service_version(const std::string& version) {
		std::scoped_lock guard(m_Mutex);
		m_ServiceVersion = version;
	}
}