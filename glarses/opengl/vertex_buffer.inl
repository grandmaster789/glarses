#ifndef GLARSES_OPENGL_VERTEX_BUFFER_INL
#define GLARSES_OPENGL_VERTEX_BUFFER_INL

#include "vertex_buffer.h"

namespace glarses::opengl {
    template <typename T>
    void VertexBuffer::store(
            std::span<T> vertices,
            GLbitfield   flags
    ) {
        assert(m_Handle != 0);

        // https://docs.gl/gl4/glBufferStorage

        glNamedBufferStorage(
                m_Handle,
                vertices.size_bytes(),
                vertices.data(),
                flags
        );
    }
}

#endif