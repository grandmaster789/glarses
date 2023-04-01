#include "shader.h"

#include <cassert>
#include <array>

namespace glarses::opengl {
    Shader::~Shader() {
        destroy();
    }

    Shader::Shader(Shader&& s) noexcept:
        m_Handle(s.m_Handle)
    {
        s.m_Handle = 0;
    }

    Shader& Shader::operator = (Shader&& s) noexcept {
        destroy();

        m_Handle = s.m_Handle;
        s.m_Handle = 0;

        return *this;
    }

    GLuint Shader::get_handle() const {
        return m_Handle;
    }

    bool Shader::compile(
            GLenum shader_type,
            std::string_view code
    ) {
        assert(m_Handle == 0);

        // https://docs.gl/gl4/glCreateShader
        m_Handle = glCreateShader(shader_type);
        const char* ptr_code = code.data();

        // https://docs.gl/gl4/glShaderSource
        glShaderSource(m_Handle, 1, &ptr_code, nullptr);

        // https://docs.gl/gl4/glCompileShader
        glCompileShader(m_Handle);

        if (!is_valid()) {
            store_error();
            destroy();

            return false;
        }

        return true;
    }

    [[nodiscard]]
    bool Shader::is_valid() const {
        assert(m_Handle != 0);

        int result = 0;

        // https://docs.gl/gl4/glGetShader
        glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &result);

        return (result == GL_TRUE);
    }

    [[nodiscard]]
    std::string_view Shader::get_error() const {
        return m_LastError;
    }

    void Shader::store_error() {
        assert(m_Handle != 0);

        std::array<char, 1024> buffer = {};
        glGetShaderInfoLog(m_Handle, static_cast<GLsizei>(buffer.size()), nullptr, buffer.data());

        m_LastError = std::string(buffer.data());
    }

    void Shader::destroy() {
        if (m_Handle) {
            glDeleteShader(m_Handle);
            m_Handle = 0;
        }
    }
}