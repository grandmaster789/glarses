#include "vertex.h"

namespace glarses::opengl {
    VertexAttribute Vertex::s_PositionAttribute = {
            3,                                                // number of elements
            GL_FLOAT,                                         // element type
            GL_FALSE,                                         // normalized?
            sizeof(Vertex),                                   // stride
            static_cast<GLuint>(offsetof(Vertex, m_Position)) // struct offset
    };

    VertexAttribute Vertex::s_ColorAttribute = {
            3,                                                // number of elements
            GL_FLOAT,                                         // element type
            GL_FALSE,                                         // normalized?
            sizeof(Vertex),                                   // stride
            static_cast<GLuint>(offsetof(Vertex, m_Color))    // struct offset
    };
}