#pragma once

#ifndef GLARSES_SIMPLE_SHADER_PROGRAM_H
#define GLARSES_SIMPLE_SHADER_PROGRAM_H

#include "../opengl/shader.h"
#include "../opengl/shader_program.h"

namespace glarses::scene {
    // FIXME this really should not be a singleton...
    class SimpleShaderProgram:
        public opengl::ShaderProgram
    {
    public:
        static SimpleShaderProgram& instance();

        void init(); // initialization requires an active openGL context

    private:
        SimpleShaderProgram() = default;

        SimpleShaderProgram             (const SimpleShaderProgram&)     = delete;
        SimpleShaderProgram& operator = (const SimpleShaderProgram&)     = delete;
        SimpleShaderProgram             (SimpleShaderProgram&&) noexcept = delete;
        SimpleShaderProgram& operator = (SimpleShaderProgram&&) noexcept = delete;
    };
}

#endif //GLARSES_SIMPLE_SHADER_PROGRAM_H
