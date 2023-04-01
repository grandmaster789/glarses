#include "shader_program.h"
#include "shader.h"

#include <array>

namespace glarses::opengl {
    ShaderProgram::~ShaderProgram() {
        if (m_Handle)
            glDeleteProgram(m_Handle);
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& sp) noexcept:
        m_Handle(sp.m_Handle)
    {
        sp.m_Handle = 0;
    }

    ShaderProgram& ShaderProgram::operator = (ShaderProgram&& sp) noexcept {
        if (m_Handle)
            glDeleteProgram(m_Handle);

        m_Handle = sp.m_Handle;

        sp.m_Handle = 0;

        return *this;
    }

    GLuint ShaderProgram::get_handle() const {
        return m_Handle;
    }

    bool ShaderProgram::compile(
        std::string_view vertex_shader_code,
        std::string_view fragment_shader_code
    ) {
        assert(m_Handle == 0);

        Shader vtx;
        Shader frag;

        if (!vtx.compile(GL_VERTEX_SHADER, vertex_shader_code)) {
            m_LastError = vtx.get_error();
            return false;
        }

        if (!frag.compile(GL_FRAGMENT_SHADER, fragment_shader_code)) {
            m_LastError = frag.get_error();
            return false;
        }

        m_Handle = glCreateProgram();

        // attach and link
        glAttachShader(m_Handle, vtx.get_handle());
        glAttachShader(m_Handle, frag.get_handle());
        glLinkProgram(m_Handle);

        if (!is_ready()) {
            std::array<char, 1024> buffer = {};

            glGetProgramInfoLog(m_Handle, static_cast<GLsizei>(buffer.size()), nullptr, buffer.data());

            m_LastError = buffer.data();

            return false;
        }

        return true;
    }

    bool ShaderProgram::is_ready() const {
        assert(m_Handle != 0);

        int status = 0;

        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);

        return (status == GL_TRUE);
    }

    void ShaderProgram::use() const {
        assert(m_Handle != 0);

        glUseProgram(m_Handle);
    }

    std::string_view ShaderProgram::get_last_error() const {
        return m_LastError;
    }

    GLint ShaderProgram::get_attribute_location(std::string_view name) const {
        assert(m_Handle != 0);

        return glGetAttribLocation(m_Handle, name.data());
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            int value
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform1i(location, value);
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            unsigned int value
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform1ui(location, value);
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            float value
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform1f(location, value);
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            const glm::vec2& value
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            const glm::vec3& value
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            const glm::vec4& value
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    void ShaderProgram::set_uniform(
            std::string_view name,
            const glm::mat4x4& matrix
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        // assume non-normalized data, 1 entry
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::set_uniform(
            std::string_view   name,
            std::span<glm::vec2> vectors
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform2fv(
                location,
                static_cast<GLsizei>(vectors.size()),
                glm::value_ptr(vectors.front())
        );
    }

    void ShaderProgram::set_uniform(
            std::string_view   name,
            std::span<glm::vec3> vectors
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform3fv(
                location,
                static_cast<GLsizei>(vectors.size()),
                glm::value_ptr(vectors.front())
        );
    }

    void ShaderProgram::set_uniform(
            std::string_view   name,
            std::span<glm::vec4> vectors
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        glUniform4fv(
                location,
                static_cast<GLsizei>(vectors.size()),
                glm::value_ptr(vectors.front())
        );
    }

    void ShaderProgram::set_uniform(
            std::string_view       name,
            std::span<glm::mat4x4> matrices
    ) {
        assert(m_Handle != 0);

        auto location = glGetUniformLocation(m_Handle, name.data());

        // assume non-normalized data, N entries from the front of the span
        glUniformMatrix4fv(
                location,
                static_cast<GLsizei>(matrices.size()),
                GL_FALSE,
                glm::value_ptr(matrices.front())
        );
    }
}