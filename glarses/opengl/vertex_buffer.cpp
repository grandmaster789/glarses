#include "vertex_buffer.h"

namespace glarses::opengl {
    VertexBuffer::~VertexBuffer() {
        if (m_Handle)
            glDeleteBuffers(1, &m_Handle);
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& vb) noexcept:
        m_Handle(vb.m_Handle)
    {
        vb.m_Handle = 0;
    }

    VertexBuffer& VertexBuffer::operator = (VertexBuffer&& vb) noexcept {
        if (m_Handle)
            glDeleteBuffers(1, &m_Handle);

        m_Handle = vb.m_Handle;

        vb.m_Handle = 0;

        return *this;
    }

    GLuint VertexBuffer::get_handle() const {
        return m_Handle;
    }

    void VertexBuffer::init() {
        assert(m_Handle == 0);

        glCreateBuffers(1, &m_Handle);
    }
}