#pragma once

#ifndef GLARSES_OPENGL_VERTEX_ARRAY_H
#define GLARSES_OPENGL_VERTEX_ARRAY_H

#include "../dependencies.h"

namespace glarses::opengl {
    class VertexBuffer;

    struct VertexMember {
        GLint     m_Size;
        GLenum    m_Type;
        GLboolean m_Normalized;
        GLsizei   m_Stride;
        GLuint    m_Offset;
    };

    class VertexArray {
    public:
        VertexArray() = default;

        VertexArray             (const VertexArray&) = delete;
        VertexArray& operator = (const VertexArray&) = delete;
        VertexArray             (VertexArray&& va) noexcept;
        VertexArray& operator = (VertexArray&& va) noexcept;

        [[nodiscard]] GLuint get_handle() const;

        void init();

               void bind();
        static void unbind();

        void attach_buffer(
                const VertexBuffer& data,
                GLuint              attribute_index,
                const VertexMember& attribute_desc
        );

    private:
        GLuint m_Handle;
    };
}

#endif