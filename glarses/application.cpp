#include "application.h"
#include "util/algorithm.h"
#include "core/system.h"
#include "core/engine.h"
#include "log/logger.h"

#include <chrono>

namespace {
    void glfw_error_callback(int error_code, const char* description) {
        g_LogError << "GLFW error[" << error_code << "]: " << description << '\n';
    }
}

namespace glarses {
	Application::Application():
        System("Glarses")
    {
        // GLFW initialization https://www.glfw.org/docs/3.3/intro_guide.html

        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");
	}

	Application::~Application() {
        glfwTerminate();
	}

    bool Application::init() {
        // create a default window
        m_Windows.emplace_back(1280, 720, "Glarses");

        return true;
    }

	void Application::update() {
        // overall application loop
        glfwPollEvents();

        std::erase_if(m_Windows, [](const os::Window& w) {
            return w.should_close();
        });

        if (m_Windows.empty())
            m_Engine->stop();
	}
}