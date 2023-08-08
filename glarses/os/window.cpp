#include "window.h"
#include <iostream>

namespace glarses::os {
    void on_framebuffer_size_callback(
            GLFWwindow* handle,
            int         width,
            int         height
    ) {
        auto obj = reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));

        obj->resize(width, height);
    }

    Window::Window(
            int width,
            int height,
            std::string_view title
    ):
        m_Width(width),
        m_Height(height),
        m_Title(title)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Handle = glfwCreateWindow(
                width,
                height,
                title.data(),
                nullptr,          // monitor
                nullptr           // shared (openGL) context
        );

        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, on_framebuffer_size_callback);
    }

    Window::~Window() {
        if (m_Handle)
            glfwDestroyWindow(m_Handle);
    }

    Window::Window(Window&& w) noexcept:
        m_Handle(w.m_Handle),
        m_Width (w.m_Width),
        m_Height(w.m_Height),
        m_Title (std::move(w.m_Title))
    {
        w.m_Handle = nullptr;
    }

    Window& Window::operator = (Window&& w) noexcept {
        if (m_Handle)
            glfwDestroyWindow(m_Handle);

        m_Handle = w.m_Handle;
        m_Width  = w.m_Width;
        m_Height = w.m_Height;
        m_Title  = std::move(w.m_Title);

        w.m_Handle = nullptr;

        return *this;
    }

    GLFWwindow* Window::get_handle() const {
        return m_Handle;
    }

    bool Window::should_close() const {
        return !!glfwWindowShouldClose(m_Handle);
    }

    void Window::resize(int width, int height) {
        m_Width      = width;
        m_Height     = height;
        m_WasResized = true;

        //std::cout << "Window resized to " << width << " x " << height << '\n';
    }

    bool Window::was_resized() const {
        return m_WasResized;
    }

    void Window::reset_resized_flag() {
        m_WasResized = false;
    }

    vk::Extent2D Window::getExtent() const {
        return {
            static_cast<uint32_t>(m_Width),
            static_cast<uint32_t>(m_Height)
        };
    }
}