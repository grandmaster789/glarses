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

	class Window:
		public util::MessageHandler<t5::Manager::GlassesFound>
	{
	public:
		Window();
		~Window();

		Window             (const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Window             (Window&&) noexcept = default;
		Window& operator = (Window&&) noexcept = default;

		void run();

		void operator()(const t5::Manager::GlassesFound& found);

	private:
		void        init_found_glasses();
		std::string make_glasses_name();

		GLFWwindow* m_Handle = nullptr;

		// TODO the stuff below here should be moved somewhere else, it's not really window-related
		ShaderProgram m_ShaderProgram;
		Texture       m_Texture;
		UniformBuffer m_PerFrameBuffer;

		std::mutex                m_FoundGlassesMutex;
		std::vector<t5::Glasses*> m_FoundGlasses;
	};
}