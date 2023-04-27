#pragma once

#ifndef GLARSES_SCENE_CUBE_H
#define GLARSES_SCENE_CUBE_H

#include "../opengl/vertex_buffer.h"
#include "../opengl/vertex_array.h"

namespace glarses::scene {
    class Cube {
    public:
        Cube() = default;

        // initializing the cube requires a valid openGL context
        void init();


        Cube             (const Cube&) = delete;
        Cube& operator = (const Cube&) = delete;
        Cube             (Cube&& c) noexcept = default;
        Cube& operator = (Cube&& c) noexcept = default;

    private:
        opengl::VertexBuffer m_Vertices;
        opengl::VertexArray  m_VAO;
    };
}

#endif