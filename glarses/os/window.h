#ifndef GLARSES_OS_WINDOW_H
#define GLARSES_OS_WINDOW_H

#include "../dependencies.h"
#include <string>
#include <string_view>

namespace glarses::os {
    class Window {
    public:
        Window() = default;
        Window(int width, int height, std::string_view title);
        ~Window();

        Window             (const Window&) = delete;
        Window& operator = (const Window&) = delete;
        Window             (Window&& w) noexcept;
        Window& operator = (Window&& w) noexcept;

        [[nodiscard]] GLFWwindow* get_handle() const;

        [[nodiscard]] bool should_close() const;

                      void resize(int width, int height);
        [[nodiscard]] bool was_resized() const;
                      void reset_resized_flag();


        [[nodiscard]] vk::Extent2D getExtent() const;

    private:
        GLFWwindow* m_Handle = nullptr;

        int  m_Width       = 0;
        int  m_Height      = 0;
        bool m_WasResized  = false;

        std::string m_Title;
    };
};

#endif
