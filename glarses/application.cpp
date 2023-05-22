#include "application.h"
#include "t5/t5_glasses.h"

namespace {
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
	Application::Application() {
		glfwSetErrorCallback(&glfw_error_callback);

		if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

		// set up an openGL 4.6 window with the Core profile
		glfwWindowHint(GLFW_CLIENT_API,            GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SRGB_CAPABLE,          GL_TRUE); // enable sRGB
		glfwWindowHint(GLFW_RESIZABLE,             GL_FALSE); // we're just doing the tilt five native resolution
	}

	Application::~Application() {
		glfwTerminate();
	}

	void Application::run() {

	}
}