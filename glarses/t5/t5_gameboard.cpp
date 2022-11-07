#include "t5_gameboard.h"

#include <ostream>

namespace t5 {
	GameboardSize::GameboardSize(const T5_GameboardSize& gs) :
		m_Size(gs)
	{
	}

	const T5_GameboardSize& GameboardSize::get() const {
		return m_Size;
	}

	float GameboardSize::get_width() const {
		return 
			m_Size.viewableExtentNegativeX + 
			m_Size.viewableExtentPositiveX;
	}

	float GameboardSize::get_height() const {
		return
			m_Size.viewableExtentPositiveZ;
	}

	float GameboardSize::get_length() const {
		return
			m_Size.viewableExtentNegativeY +
			m_Size.viewableExtentPositiveY;
	}

	std::ostream& operator << (std::ostream& os, const GameboardSize& sz) {
		os 
			<< '<'    << sz.get_width() 
			<< "m x " << sz.get_length()
			<< "m x " << sz.get_height()
			<< "m>";

		return os;
	}
}