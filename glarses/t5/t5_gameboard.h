#pragma once

#include "../dependencies.h"

#include <iosfwd>

namespace t5 {
	class GameboardSize {
	public:
		GameboardSize() = default;

		explicit GameboardSize(const T5_GameboardSize& gs);

		const T5_GameboardSize& get() const;

		float get_width() const;
		float get_height() const;
		float get_length() const;

		friend std::ostream& operator << (std::ostream& os, const GameboardSize& sz);

	private:
		T5_GameboardSize m_Size = {};
	};

	class Gameboard {
	public:

	};
}