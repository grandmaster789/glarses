#include "window.h"
#include "dependencies.h"
#include "cube.h"
#include "io/file.h"

#include <stdexcept>
#include <iostream>
#include <array>
#include <string>

namespace {
	bool g_HaveWindow = false;
	
	std::string intepret_glfw_errorcode(int code) {
		switch (code) {
		case GLFW_NOT_INITIALIZED:		return "GLFW has not been initialized";
		case GLFW_NO_CURRENT_CONTEXT:	return "No context is current for this thread";
		case GLFW_INVALID_ENUM:			return "One of the arguments to the function was an invalid enum value";
		case GLFW_INVALID_VALUE:		return "One of the arguments to the function was an invalid value";
		case GLFW_OUT_OF_MEMORY:		return "A memory allocation failed";
		case GLFW_API_UNAVAILABLE:		return "GLFW could not find support for the requested client API on the system";
		case GLFW_VERSION_UNAVAILABLE:	return "The requested OpenGL or OpenGL ES version is not available";
		case GLFW_PLATFORM_ERROR:		return "A platform - specific error occurred that does not match any of the more specific categories";
		case GLFW_FORMAT_UNAVAILABLE:	return "The requested format is not supported or available";
		default:
			return std::string("Unknown error code: ") + std::to_string(code);
		}
	}

	void glfw_error_callback(int code, const char* description) {
		std::cerr << "GLFW error (" << intepret_glfw_errorcode(code) << "): " << description << '\n';
	}
}

namespace glarses {
	Window::Window() {
		if (g_HaveWindow)
			throw std::runtime_error("Only one window is allowed...");
		else
			g_HaveWindow = true;

		glfwSetErrorCallback(&glfw_error_callback);

		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");

		// set up an openGL 4.3 window with the Core profile
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Handle = glfwCreateWindow(
			800,		// width
			600,        // height
			"Glarses",  // title
			nullptr,    // monitor
			nullptr     // shared opengl context
		);

		if (m_Handle == nullptr)
			throw std::runtime_error("Failed to create a window");

		glfwMakeContextCurrent(m_Handle);
		glfwSwapInterval(1); // wait for 1 screen update before swapping front/back buffers (vsync)

		glfwSetInputMode(m_Handle, GLFW_STICKY_KEYS, GL_TRUE); // buffer the keypresses

		glewExperimental = true; // this is actually required for glew to try and get openGL core functions
		auto glew_err = glewInit();
		if (glew_err != GLEW_OK) {
			std::cerr << glewGetErrorString(glew_err) << '\n';
			throw std::runtime_error("Failed to initialize GLEW");
		}
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::run() {
		glClearColor(0.2f, 0.0f, 0.4f, 0.0f); // purple

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		bool done = false;

		while (!done) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwSwapBuffers(m_Handle);

			glfwPollEvents();

			done |= (glfwWindowShouldClose(m_Handle) == GL_TRUE);          // someone closed the window
			done |= (glfwGetKey(m_Handle, GLFW_KEY_ESCAPE) == GLFW_PRESS); // someone pressed escape
		}
	}
}