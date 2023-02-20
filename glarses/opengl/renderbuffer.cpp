#include "renderbuffer.h"

namespace glarses::opengl {
    RenderBuffer::~RenderBuffer() {
        destroy();
    }

    RenderBuffer::RenderBuffer(RenderBuffer&& rb) noexcept:
        m_Handle(rb.m_Handle)
    {
        rb.m_Handle = 0;
    }

    RenderBuffer& RenderBuffer::operator = (RenderBuffer&& rb) noexcept {
        destroy();

        m_Handle = rb.m_Handle;

        rb.m_Handle = 0;

        return *this;
    }

    void RenderBuffer::set_color_storage  (
        GLenum format,
        GLsizei width,
        GLsizei height
    ) {
        assert(!m_Handle);

        glNamedRenderbufferStorage(
            m_Handle,
            format,
            width,
            height
        );
    }

    void RenderBuffer::set_depth_storage  (
        GLsizei width,
        GLsizei height
    ) {
        assert(!m_Handle);

        // https://docs.gl/gl4/glRenderbufferStorage
        glNamedRenderbufferStorage(
            m_Handle,
            GL_DEPTH_COMPONENT,
            width,
            height
        );
    }

    void RenderBuffer::set_stencil_storage(
        GLsizei width,
        GLsizei height
    ) {
        assert(!m_Handle);

        // https://docs.gl/gl4/glRenderbufferStorage
        glNamedRenderbufferStorage(
            m_Handle,
            GL_STENCIL_COMPONENTS,
            width,
            height
        );
    }

    void RenderBuffer::create() {
        assert(!m_Handle);
        glCreateRenderbuffers(1, &m_Handle);
    }

    void RenderBuffer::destroy() {
        if (m_Handle)
            glDeleteRenderbuffers(1, &m_Handle);
    }

    [[nodiscard]]
    GLuint RenderBuffer::get_handle() const {
        return m_Handle;
    }

    void RenderBuffer::bind(GLenum target) const {
        glBindRenderbuffer(target, m_Handle);
    }

    void RenderBuffer::unbind(GLenum target) const {
        glBindRenderbuffer(target, 0);
    }
}