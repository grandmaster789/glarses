#pragma once

#ifndef GLARSES_VERTEX_ATTRIBUTE_H
#define GLARSES_VERTEX_ATTRIBUTE_H

#include "../dependencies.h"

namespace glarses::opengl {
    struct VertexAttribute {
        GLint     m_NumElements;
        GLenum    m_Type;
        GLboolean m_Normalized;
        GLsizei   m_Stride;
        GLuint    m_Offset;
    };
}

#endif
