#pragma once

#include "../dependencies.h"
#include "../math/transform.h"

#include <string_view>
#include <mutex>

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

		explicit Glasses(const std::string& hardware_id);

		Glasses() = default;
		~Glasses();

		Glasses             (const Glasses&) = delete;
		Glasses& operator = (const Glasses&) = delete;
		Glasses             (Glasses&& g) noexcept;
		Glasses& operator = (Glasses&& g) noexcept;

        bool init(
            std::string_view display_name,
            GLFWwindow*      window_handle
        );

        glm::vec3         get_position()    const; // relative to gameboard (xy is gameboard plane)
        glm::quat         get_orientation() const; //
        T5_GameboardType  get_board_type()  const;

	private:
        friend class Manager;

        void poll(); // updates pose, called periodically by the t5_manager (from the managers' thread)
        void cleanup() noexcept;

        [[nodiscard]] T5_ConnectionState query_connection_state() const;
        [[nodiscard]] double             query_ipd() const;

        T5_Glasses  m_Handle  = nullptr;
        std::string m_FriendlyName;
        double      m_IPD     = 0.059; // in meters

        mutable std::mutex m_Mutex;
        T5_GlassesPose     m_Pose;  // includes position, orientation, timestamp, board type
	};

    std::ostream& operator << (std::ostream& os, T5_ConnectionState state);
}