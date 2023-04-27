#pragma once

#ifndef GLARSES_VERTEX_H
#define GLARSES_VERTEX_H

#include "../dependencies.h"
#include "vertex_attribute.h"

namespace glarses::opengl {
    struct Vertex {
        glm::vec3 m_Position;
        glm::vec3 m_Color;

        static VertexAttribute s_PositionAttribute;
        static VertexAttribute s_ColorAttribute;
    };
}

#endif
