#pragma once

#include "../dependencies.h"

#include <string>
#include <string_view>

namespace glarses::opengl {
    class Shader {
    public:
        Shader() = default;
        ~Shader();

        Shader             (const Shader&) = delete;
        Shader& operator = (const Shader&) = delete;
        Shader             (Shader&& s) noexcept;
        Shader& operator = (Shader&& s) noexcept;

        [[nodiscard]] GLuint get_handle() const;

        bool compile(
            GLenum           shader_type,    // ie. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
            std::string_view code
        );                                   // if compilation fails, use get_error to obtain the error message
        [[nodiscard]] bool is_valid() const; // checks whether compilation has succeeded

        [[nodiscard]] std::string_view get_error() const;

    protected:
        void store_error();
        void destroy();

        GLuint      m_Handle = 0;
        std::string m_LastError;
    };
}