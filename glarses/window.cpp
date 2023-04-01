#include "window.h"
#include "dependencies.h"
#include "io/file.h"

#include "t5/t5_manager.h"
#include "t5/t5_glasses.h"

#include <stdexcept>
#include <iostream>
#include <array>
#include <string>

namespace {
	bool g_HaveWindow = false;

	void GLAPIENTRY opengl_debug_callback(
		GLenum,          // source
		GLenum,          // type
		GLuint,          // id
		GLenum,          // severity
		GLsizei,         // message size
		GLchar* message,
		const void*      // userParam
	) {
		std::cerr << "OpenGL debug message: " << message << '\n';
	}

	std::filesystem::path find_asset_folder() {
		using namespace std::filesystem;

		auto pwd = current_path();

		while (!pwd.empty()) {
			if (exists(pwd / "assets"))
				return pwd / "assets";

			pwd = pwd.parent_path();
		}

		throw std::runtime_error("Failed to find the asset folder");
	}
}

namespace glarses {
	Window::Window(int width, int height) {
		// NOTE the one window thing is mostly due to the glew initialization... it should be possible to get rid of it but it's not a priority
		//      (also, at the time of writing, tilt five only supports one pair of glasses per computer)
		if (g_HaveWindow)
			throw std::runtime_error("Only one window is allowed...");
		else
			g_HaveWindow = true;

		// TiltFive native resolution is 1216x768 per eye
		// (expected texture format is R8G8B8A8)
		m_Handle = glfwCreateWindow(
			width,		// width
			height,     // height
			"Glarses",  // title
			nullptr,    // monitor
			nullptr     // shared opengl context
		);

		if (m_Handle == nullptr)
			throw std::runtime_error("Failed to create a window");

		glfwMakeContextCurrent(m_Handle);
		glfwSwapInterval(1); // wait for 1 screen update before swapping front/back buffers (vsync)

		glfwSetInputMode(m_Handle, GLFW_STICKY_KEYS, GL_TRUE); // buffer the key presses

		// glew requires an active context in order to initialize properly (and thus can't be moved to the application startup)

		glewExperimental = true; // this is actually required for glew to try and get openGL core functions
		auto glew_err = glewInit();
		if (glew_err != GLEW_OK) {
			std::cerr << glewGetErrorString(glew_err) << '\n';
			throw std::runtime_error("Failed to initialize GLEW");
		}

		// after extensions are enabled we can enable debug callbacks
#ifdef _DEBUG
		glDebugMessageCallbackARB(reinterpret_cast<GLDEBUGPROCARB>(opengl_debug_callback), nullptr);
		if (glGetError() != GL_NO_ERROR)
			std::cerr << "glDebugMessageCallbackARB failure\n";

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

		// disable the 'notification' severity
		glDebugMessageControlARB(
			GL_DONT_CARE,	        		// source
			GL_DONT_CARE,					// type
			GL_DEBUG_SEVERITY_NOTIFICATION,	// severity
			0,								// count
			nullptr,						// GLuint *ids
			GL_FALSE						// enabled
		);
#endif
		
		auto assets = find_asset_folder();

		glClearColor(0.2f, 0.0f, 0.4f, 0.0f); // purple
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}

	GLFWwindow* Window::get_handle() const {
		return m_Handle;
	}

	/*
	void Window::run() {
		bool done = false;

		// create a very temporary vertex array object without any data -- the required data is embedded in the shaders
		GLuint vao = 0;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		while (!done) {
			int frame_width = 0;
			int frame_height = 0;

			glfwGetFramebufferSize(m_Handle, &frame_width, &frame_height);
			float ratio = static_cast<float>(frame_width) / std::max(frame_height, 1);

			auto time_since_app_launch = static_cast<float>(glfwGetTime());

			glViewport(0, 0, frame_width, frame_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// rotate the triangle around the Z axis
			glm::mat4 model_matrix = glm::rotate(
				glm::mat4(1.0f),
				time_since_app_launch,
				glm::vec3(0.0f, 0.0f, 1.0f)
			);

			// orthographic (flat) projection
			glm::mat4 projection_matrix = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

			// we don't need a view (camera) matrix yet
			auto mvp = projection_matrix * model_matrix;

			m_ShaderProgram.use();

			m_PerFrameBuffer.set_data(glm::value_ptr(mvp));

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(m_Handle);
			glfwPollEvents();

			done |= (glfwWindowShouldClose(m_Handle) == GL_TRUE);          // someone closed the window
			done |= (glfwGetKey(m_Handle, GLFW_KEY_ESCAPE) == GLFW_PRESS); // someone pressed escape
		}

		glDeleteVertexArrays(1, &vao);
	}
	*/

	void Window::make_current() {
		glfwMakeContextCurrent(m_Handle);
	}

	void Window::clear() {
		int frame_width = 0;
		int frame_height = 0;

		glfwGetFramebufferSize(m_Handle, &frame_width, &frame_height);

		glViewport(0, 0, frame_width, frame_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::swap_buffers() {
		glfwSwapBuffers(m_Handle);
	}

	bool Window::should_close() const {
		bool done = false;

		done |= (glfwWindowShouldClose(m_Handle) == GL_TRUE);
		done |= (glfwGetKey(m_Handle, GLFW_KEY_ESCAPE) == GLFW_PRESS);

		return done;
	}
}