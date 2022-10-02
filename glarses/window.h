#pragma once

#include "dependencies.h"

#include "shader_program.h"
#include "texture.h"
#include "uniform_buffer.h"

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

		// TODO the stuff below here should be moved somewhere else, it's not really window-related
		ShaderProgram m_ShaderProgram;
		Texture       m_Texture;
		UniformBuffer m_PerFrameBuffer;
	};
}