#pragma once

#include "dependencies.h"

#include "opengl/shader_program.h"
#include "opengl/texture.h"
#include "opengl/uniform_buffer.h"

#include "t5/t5_manager.h"

#include "util/mediator.h"

#include <mutex>

namespace glarses {
	// kind of sloppy design - probably should separate various context interactions from the window, but hey... 
	// just don't create multiple windows and you'll be fine (very basic guards are in place)

	// When possible, the window should display one of the renders for an eye of the tiltfive

	class Window {
	public:
		Window(int width, int height);

		Window             (const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Window             (Window&&) noexcept = default;
		Window& operator = (Window&&) noexcept = default;

		GLFWwindow* get_handle() const;

		void make_current();
		void clear();
		void swap_buffers();
		bool should_close() const;

	private:
		GLFWwindow* m_Handle = nullptr;

		// TODO the stuff below here should be moved somewhere else, it's not really window-related
		ShaderProgram m_ShaderProgram;
		Texture       m_Texture;
		UniformBuffer m_PerFrameBuffer;
	};
}