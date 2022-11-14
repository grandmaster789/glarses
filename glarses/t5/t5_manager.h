#pragma once

#include <thread>
#include <string>
#include <mutex>
#include <memory>

#include "../dependencies.h"
#include "../util/flat_map.h"

namespace glarses {
	class Window;
}

namespace glarses::t5 {
	class Glasses;

	class Manager {
	public:
		// Helpers start additional background threads to continuously poll
		using millisec   = std::chrono::milliseconds;
		using GlassesPtr = std::unique_ptr<Glasses>;
		
	private:
		Manager();

	public:
		~Manager();

		Manager             (const Manager&) = delete;
		Manager& operator = (const Manager&) = delete;
		Manager             (Manager&&) noexcept = delete;
		Manager& operator = (Manager&&) noexcept = delete;
		
		static Manager& instance();
		void destroy();

		const std::string& get_application_id() const;
		const std::string& get_service_version() const;
		T5_Context         get_context() const;

		struct GlassesFound { std::string m_HardwareID; Glasses* m_Glasses = nullptr; };
		struct GlassesLost  { std::string m_HardwareID; Glasses* m_Glasses = nullptr; };

	private:
		static constexpr millisec k_PollingRate = millisec(10);

		bool init_client();
		void update_glasses_list();
		void set_service_version(const std::string& version);

		mutable std::mutex m_Mutex;
		std::jthread       m_Thread;
		
		bool               m_Exiting = false;

		std::string m_ApplicationID      = "com.grandmaster.glarses";
		std::string m_ApplicationVersion = "0.0.1";
		std::string m_ServiceVersion;

		T5_Context m_Context = nullptr;

		util::FlatMap<std::string, GlassesPtr> m_Glasses; // 'hardware' id -> Glasses object (owning)
	};
}