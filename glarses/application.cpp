#include "application.h"
#include "util/algorithm.h"

namespace {
    void glfw_error_callback(int error_code, const char* description) {
        std::cerr << "GLFW error[" << error_code << "]: " << description << '\n';
    }
}

namespace glarses {
	Application::Application() {
        // GLFW initialization https://www.glfw.org/docs/3.3/intro_guide.html

        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");
	}

	Application::~Application() {
        glfwTerminate();
	}

	void Application::run() {


        // create a default window
        m_Windows.emplace_back(1280, 720, "Glarses");

        // overall application loop
        while (!m_Windows.empty()) {
            glfwPollEvents();

            std::erase_if(m_Windows, [](const os::Window& w) {
                return w.should_close();
            });
        }
	}
}