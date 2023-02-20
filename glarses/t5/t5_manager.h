#pragma once

#include <thread>
#include <string>
#include <string_view>
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
        using Clock      = std::chrono::high_resolution_clock;
		using millisec   = std::chrono::milliseconds;
        using TimePoint  = Clock::time_point;
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

        // at the 'system' level there are currently only 2 parameters to query
        // 1: version of the service software
        // 2: control panel requires user interaction
        //
        // Both are unlikely to change while running this application, and the second one
        // can be considered out-of-scope. We'll just query the service version during
        // init and cache the results.
        //
		std::string_view get_application_id() const;
		std::string_view get_service_version() const;

		T5_Context       get_context() const;

		struct GlassesFound { std::string m_HardwareID; Glasses* m_Glasses = nullptr; };
		struct GlassesLost  { std::string m_HardwareID; Glasses* m_Glasses = nullptr; };

	private:
		static constexpr millisec k_GlassesPollingRate = millisec(10);   // try to go for 100Hz updates for the glasses themselves

		bool init_client();
		void update_glasses_list();
        void poll_glasses();

		void set_service_version(std::string_view version);

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