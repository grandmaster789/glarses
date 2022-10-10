#include "t5_context.h"
#include "../dependencies.h"

#include <iostream>
#include <format>

namespace t5 {
	Context::Context() {
		auto client = tiltfive::obtainClient("com.tiltfive.test", "0.1.0", nullptr);
		if (!client)
			throw std::runtime_error("Failed to create client");
		
		std::cout << "Obtained client: " << client << '\n';

		m_Client = std::move(*client); // unwrap the Result

		print_gameboard_dimensions();
	}

	void Context::print_gameboard_dimensions() {
		auto sz = m_Client->getGameboardSize(kT5_GameboardType_XE_Raised); // ? that doesn't seem right

		if (!sz)
			std::cout << "Failed to get gameboard dimensions\n";
		else {
			// the negative direction contains positive values :P
			float width  = sz->viewableExtentPositiveX + sz->viewableExtentNegativeX;
			float length = sz->viewableExtentPositiveY + sz->viewableExtentNegativeY;
			float height = sz->viewableExtentPositiveZ;

			std::cout << std::format("{} x {} x {}\n", width, length, height);
		}
	}
}