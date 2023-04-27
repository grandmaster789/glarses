#pragma once

#ifndef GLARSES_OPENGL_SHADER_PROGRAM_H
#define GLARSES_OPENGL_SHADER_PROGRAM_H

#include "../dependencies.h"

#include <string_view>
#include <span>

namespace glarses::opengl {
    class ShaderProgram {
    public:
        ShaderProgram() = default;
        ~ShaderProgram();

        ShaderProgram             (const ShaderProgram&) = delete;
        ShaderProgram& operator = (const ShaderProgram&) = delete;
        ShaderProgram             (ShaderProgram&& s) noexcept;
        ShaderProgram& operator = (ShaderProgram&& s) noexcept;

        [[nodiscard]] GLuint get_handle() const;

        bool compile(
                std::string_view vertex_shader_code,
                std::string_view fragment_shader_code
        );
        [[nodiscard]] bool             is_ready() const; // whether the program has linked and can be used
                      void             use() const;
        [[nodiscard]] std::string_view get_last_error() const;

        [[nodiscard]] GLint            get_attribute_location(std::string_view name) const;

        void set_uniform(std::string_view name, int                    value);
        void set_uniform(std::string_view name, unsigned int           value);
        void set_uniform(std::string_view name, float                  value);

        void set_uniform(std::string_view name, const glm::vec2&       value);
        void set_uniform(std::string_view name, const glm::vec3&       value);
        void set_uniform(std::string_view name, const glm::vec4&       value);
        void set_uniform(std::string_view name, const glm::mat4x4&     matrix);

        void set_uniform(std::string_view name, std::span<glm::vec2>   vectors);
        void set_uniform(std::string_view name, std::span<glm::vec3>   vectors);
        void set_uniform(std::string_view name, std::span<glm::vec4>   vectors);
        void set_uniform(std::string_view name, std::span<glm::mat4x4> matrices);

    protected:
        GLuint      m_Handle = 0; // created in :compile(...)
        std::string m_LastError;
    };
}

#endif