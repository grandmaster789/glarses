#pragma once

#include "dependencies.h"

namespace glarses {
	// kind of sloppy design - probably should separate various context interactions from the window, but hey... 
	// just don't create multiple windows and you'll be fine (very basic guards are in place)
	// also - this 

	class Window {
	public:
		Window();
		~Window();

		Window             (const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Window             (Window&&) noexcept = default;
		Window& operator = (Window&&) noexcept = default;

		void run();

	private:
		GLFWwindow* m_Handle = nullptr;
	};
}