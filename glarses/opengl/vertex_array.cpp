#include "vertex_array.h"
#include "vertex_attribute.h"
#include "vertex_buffer.h"

namespace glarses::opengl {
    VertexArray::VertexArray(VertexArray&& va) noexcept:
        m_Handle(va.m_Handle)
    {
        va.m_Handle = 0;
    }

    VertexArray& VertexArray::operator = (VertexArray&& va) noexcept {
        if (m_Handle)
            // https://docs.gl/gl4/glDeleteVertexArrays
            glDeleteVertexArrays(1, &m_Handle);

        m_Handle = va.m_Handle;

        va.m_Handle = 0;

        return *this;
    }

    GLuint VertexArray::get_handle() const {
        return m_Handle;
    }

    void VertexArray::init() {
        assert(m_Handle == 0);

        // https://docs.gl/gl4/glCreateVertexArrays
        glCreateVertexArrays(1, &m_Handle);
    }

    void VertexArray::bind() {
        // https://docs.gl/gl4/glBindVertexArray
        glBindVertexArray(m_Handle);
    }

    void VertexArray::unbind() {
        // https://docs.gl/gl4/glBindVertexArray
        glBindVertexArray(0);
    }

    void VertexArray::attach_buffer(
            const VertexBuffer&    data,
            GLuint                 attribute_index,
            const VertexAttribute& attribute_desc
    ) {
        assert(m_Handle != 0);

        // https://docs.gl/gl4/glEnableVertexAttribArray
        glEnableVertexArrayAttrib(
                m_Handle,
                attribute_index
        );

        // https://docs.gl/gl4/glBindVertexBuffer
        glVertexArrayVertexBuffer(
                m_Handle,
                0,
                data.get_handle(),
                0,
                attribute_desc.m_Stride
        );

        // https://docs.gl/gl4/glVertexAttribBinding
        glVertexArrayAttribBinding(
                m_Handle,
                attribute_index,
                0
        );

        // https://docs.gl/gl4/glVertexAttribFormat
        glVertexArrayAttribFormat(
                m_Handle,
                attribute_index,
                attribute_desc.m_Stride,
                attribute_desc.m_Type,
                attribute_desc.m_Normalized,
                attribute_desc.m_Offset
        );
    }
}