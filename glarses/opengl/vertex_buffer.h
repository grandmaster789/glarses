#ifndef GLARSES_OPENGL_VERTEX_BUFFER_H
#define GLARSES_OPENGL_VERTEX_BUFFER_H

#include "../dependencies.h"

#include <span>

namespace glarses::opengl {
    class VertexBuffer {
    public:
        VertexBuffer() = default;
        ~VertexBuffer();

        VertexBuffer             (const VertexBuffer&) = delete;
        VertexBuffer& operator = (const VertexBuffer&) = delete;
        VertexBuffer             (VertexBuffer&& vb) noexcept;
        VertexBuffer& operator = (VertexBuffer&& vb) noexcept;

        [[nodiscard]] GLuint get_handle() const;

        void init();

        // flags must be a bitwise combination of the following flags:
        //      GL_DYNAMIC_STORAGE_BIT,
        //      GL_MAP_READ_BIT GL_MAP_WRITE_BIT,
        //      GL_MAP_PERSISTENT_BIT,
        //      GL_MAP_COHERENT_BIT,
        //      and GL_CLIENT_STORAGE_BIT
        template <typename T>
        void store(
                std::span<T> vertices,
                GLbitfield   flags = 0
        );

    private:
        GLuint m_Handle = 0;
    };
}

#include "vertex_buffer.inl"

#endif