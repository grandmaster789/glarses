#pragma once

#include <string>
#include <string_view>
#include <iosfwd>

#include "../dependencies.h"

namespace t5 {
	// NOTE this wraps T5_Context
	// 
	// TODO this performs operations in exclusivity group 1
	// 
	// NOTE several of the calls are semi-blocking, depending on service availability several retries may be done
	//      I've named these functions 'attempt_*', to indicate that you should't call any of these in a hot loop
	//      
	class Glasses;
	class GameboardSize;

	class Client {
	private:
		Client() = default;

	public:
		~Client();

		Client             (const Client&)     = delete;
		Client& operator = (const Client&)     = delete;
		Client             (Client&&) noexcept = delete;
		Client& operator = (Client&&) noexcept = delete;

		static Client& create(
			std::string_view application_id,
			std::string_view application_version
		);

		static Client& instance();

		T5_Context get_handle() const;

		// ATTEMPT section (semi-blocking functions that may retry a couple of times)
		std::string              attempt_get_service_version()   const;
		bool                     attempt_is_attention_required() const;
		std::vector<std::string> attempt_list_glasses()          const; // includes glasses that are locked for use by other clients, as well as glasses not in the 'ready' state
		// /ATTEMPT

		Glasses create_glasses(
			std::string_view id,		  // as reported by attempt_list_glasses
			std::string_view display_name // e.g. "Player 1"
		) const; 

		std::vector<T5_ParamSys> get_changed_system_parameters() const; // first call will always yield an empty list
		GameboardSize            get_gameboard_size(T5_GameboardType type) const;

		friend std::ostream& operator << (std::ostream& os, const Client& clt);

	private:
		void init(
			std::string_view id,
			std::string_view ver
		);

		inline static constexpr std::chrono::milliseconds k_RetryDelay = std::chrono::milliseconds(10);
		inline static constexpr int                       k_MaxRetries = 10;

		T5_Context    m_Context    = nullptr;
		T5_ClientInfo m_ClientInfo = {};

		// TOOD
		// ~ guarantee that operations are performed on the same thread
		// ~ make functions threadsafe
	};
}