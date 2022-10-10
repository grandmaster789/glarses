#pragma once

#include "../dependencies.h"

namespace t5 {
	class Context {
	public:
		Context();

		void print_gameboard_dimensions();

	private:
		// the shared_ptr's are not great, but not *that* horrible either
		std::shared_ptr<tiltfive::Client> m_Client;

	};
}