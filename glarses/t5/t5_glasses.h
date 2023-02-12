#pragma once

#include "../dependencies.h"
#include "../opengl/render_target.h"
#include "../math/transform.h"

#include <string_view>

namespace glarses {
	class Player;
}

namespace glarses::t5 {
	class Manager;

	class Glasses {
	public:
		using Clock     = std::chrono::high_resolution_clock;
		using Timepoint = Clock::time_point;
		using millisec  = std::chrono::milliseconds;

		explicit Glasses(std::string_view hardware_id);

		Glasses() = default;
		~Glasses();

		Glasses             (const Glasses&) = delete;
		Glasses& operator = (const Glasses&) = delete;
		Glasses             (Glasses&& g) noexcept;
		Glasses& operator = (Glasses&& g) noexcept;

		[[nodiscard]] bool                  is_pose_fresh() const;
		[[nodiscard]] const T5_GlassesPose& get_pose()      const;

	private:
		friend class Player;
        friend class Manager;

		bool init(std::string_view display_name, GLFWwindow* context); // should be called from the graphics thread

		void poll();

		static constexpr millisec k_RetryTiming    = millisec(100);
		static constexpr millisec k_PoseExpiration = millisec(20);

		void release();
		bool ensure_ready(int max_retries = 20);
		bool initGLContext(GLFWwindow* context);

		void update_pose();
		void update_connection_state();
		void update_ipd();

		T5_Glasses         m_Handle    = nullptr;
		T5_Result          m_LastError = T5_SUCCESS;
		T5_GlassesPose     m_LastPose  = {};
		Timepoint          m_PoseAcquiredTimestamp;
		T5_ConnectionState m_State     = kT5_ConnectionState_Disconnected;

		double m_IPD      = 59.0; // in millimeters
		bool   m_Ready    = false;
		bool   m_Acquired = false;
	};
}