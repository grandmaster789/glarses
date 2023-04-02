#pragma once

#ifndef GLARSES_SCENE_CUBE_H
#define GLARSES_SCENE_CUBE_H

#include "../opengl/vertex_buffer.h"
#include "../opengl/vertex_array.h"

namespace glarses::scene {
    class Cube {
    public:
        Cube(float size = 1.0f);

        // default move, copy via .clone()
        Cube             (const Cube&) = delete;
        Cube& operator = (const Cube&) = delete;
        Cube             (Cube&& c) noexcept;
        Cube& operator = (Cube&& c) noexcept;

        Cube clone() const;

    private:

    };
}

#endif