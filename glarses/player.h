#pragma once

#include "t5/t5_glasses.h"
#include "window.h"

namespace glarses {
	class Player {
	public:
		Player(
			const std::string& player_name, 
			t5::Glasses*       glasses
		);

		Player             (const Player&) = delete;
		Player& operator = (const Player&) = delete;
		Player             (Player&&) noexcept = default;
		Player& operator = (Player&&) noexcept = default;

		struct Quit { Player* m_Player; };

		void update();

	private:
		static constexpr int k_TiltFiveNativeResolutionX = 1216;
		static constexpr int k_TiltFiveNativeResolutionY = 768;

		std::string  m_Name;
		t5::Glasses* m_Glasses = nullptr; // the t5 Manager actually owns the object
		
		Window m_Window = Window(k_TiltFiveNativeResolutionX, k_TiltFiveNativeResolutionY);

		opengl::RenderTarget m_LeftEye  = opengl::RenderTarget(k_TiltFiveNativeResolutionX, k_TiltFiveNativeResolutionY);
		opengl::RenderTarget m_RightEye = opengl::RenderTarget(k_TiltFiveNativeResolutionX, k_TiltFiveNativeResolutionY);
		math::Transform      m_Pose;
	};
}