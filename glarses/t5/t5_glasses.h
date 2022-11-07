#pragma once

#include <string>
#include <string_view>
#include <iosfwd>

#include "../dependencies.h"

namespace t5 {
	class Glasses {
	public:
		Glasses() = default;
		~Glasses();
		
		Glasses(
			T5_Context       ctx, 
			std::string_view glasses_id, 
			std::string_view display_name
		);

		Glasses             (const Glasses&) = delete;
		Glasses& operator = (const Glasses&) = delete;
		Glasses             (Glasses&&) noexcept;
		Glasses& operator = (Glasses&&) noexcept;

		void               set_name(std::string_view sv); // requires exclusive connection
		const std::string& get_name() const;              // as in user facing name
		std::string        get_identifier() const;        // as in hardware id (ish)

		void init_graphics()                        const; // initializes openGL context (must be called from graphics thread)
		bool try_get_pose(T5_GlassesPose* out_pose) const; // returns success/failure; needs exclusive connection

		T5_ConnectionState get_connection_state() const;
		T5_ConnectionState update_connection_state();

		friend std::ostream& operator << (std::ostream& os, const Glasses& g);

	private:
		// for exlusive operations, acquire -> ensure -> <operation> -> release
		void acquire()      const; //
		void ensure_ready() const; // may timeout
		void release()      const;

		// helper RAII that escalates from non-exclusive to connected state, and releases at end of scope
		friend struct ExclusiveHelper;
		struct ExclusiveHelper {
			ExclusiveHelper(const Glasses* g);
			~ExclusiveHelper();

			const Glasses* m_Glasses;
		};

		T5_Glasses         m_Handle      = nullptr;
		std::string        m_DisplayName = "Player 1";

		static constexpr int                       k_MaxNumRetries = 10;
		static constexpr std::chrono::milliseconds k_RetryDelay    = std::chrono::milliseconds(10);

		/*
		*	Connection states:
		*		kT5_ConnectionState_NotExclusivelyConnected	[non-exclusive]
		*		kT5_ConnectionState_ExclusiveReservation    [reserved]
		*		kT5_ConnectionState_ExclusiveConnection		[connected]
		*		kT5_ConnectionState_Disconnected			[disconnected]
		*/

		T5_ConnectionState m_State;
	};
}