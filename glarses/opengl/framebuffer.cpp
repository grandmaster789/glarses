#include "framebuffer.h"
#include "texture.h"
#include "renderbuffer.h"

namespace glarses::opengl {
    FrameBuffer::~FrameBuffer() {
        destroy();
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& fb) noexcept:
        m_Handle(fb.m_Handle)
    {
        fb.m_Handle = 0;
    }

    FrameBuffer& FrameBuffer::operator = (FrameBuffer&& fb) noexcept {
        destroy();

        m_Handle = fb.m_Handle;

        fb.m_Handle = 0;

        return *this;
    }

    void FrameBuffer::set_color_attachment(
        const Texture& t,
        int            mip_level,
        int            color_attachment_idx
    ) {
        assert(!m_Handle);

        // https://docs.gl/gl4/glFramebufferTexture
        glNamedFramebufferTexture(
            m_Handle,
            GL_COLOR_ATTACHMENT0 + color_attachment_idx,
            t.get_handle(),
            mip_level
        );
    }

    void FrameBuffer::set_depth_attachment(
        const Texture& t,
        int            mip_level
    ) {
        assert(!m_Handle);

        // https://docs.gl/gl4/glFramebufferTexture
        glNamedFramebufferTexture(
            m_Handle,
            GL_DEPTH_ATTACHMENT,
            t.get_handle(),
            mip_level
        );
    }

    void FrameBuffer::set_stencil_attachment(
        const Texture& t,
        int            mip_level
    ) {
        assert(!m_Handle);

        // https://docs.gl/gl4/glFramebufferTexture
        glNamedFramebufferTexture(
                m_Handle,
                GL_STENCIL_ATTACHMENT,
                t.get_handle(),
                mip_level
        );
    }

    void FrameBuffer::set_color_attachment(
        const RenderBuffer& rb,
        int color_attachment_idx
    ) {
        assert(!m_Handle);

        glNamedFramebufferRenderbuffer(
                m_Handle,
                GL_COLOR_ATTACHMENT0 + color_attachment_idx,
                GL_RENDERBUFFER,
                rb.get_handle()
        );
    }

    void FrameBuffer::set_depth_attachment(const RenderBuffer& rb) {
        assert(!m_Handle);

        glNamedFramebufferRenderbuffer(
                m_Handle,
                GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER,
                rb.get_handle()
        );
    }

    void FrameBuffer::set_stencil_attachment(const RenderBuffer& rb) {
        assert(!m_Handle);

        glNamedFramebufferRenderbuffer(
                m_Handle,
                GL_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER,
                rb.get_handle()
        );
    }

    void FrameBuffer::create() {
        assert(!m_Handle);

        glCreateFramebuffers(1, &m_Handle);
    }

    void FrameBuffer::destroy() {
        if (m_Handle)
            glDeleteFramebuffers(1, &m_Handle);
    }

    [[nodiscard]]
    GLuint FrameBuffer::get_handle() const {
        return m_Handle;
    }

    [[nodiscard]]
    GLenum FrameBuffer::check_status() const {
        assert(m_Handle);
        return glCheckNamedFramebufferStatus(m_Handle, GL_FRAMEBUFFER);
    }

    [[nodiscard]]
    bool FrameBuffer::is_ready() const {
        if (!m_Handle)
            return false;

        return check_status() == GL_FRAMEBUFFER_COMPLETE;
    }

    std::string FrameBuffer::status_to_string(GLenum status) {
        switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:                      return "Framebuffer complete";
        case GL_FRAMEBUFFER_UNDEFINED:                     return "Framebuffer undefined";
        case GL_FRAMEBUFFER_UNSUPPORTED:                   return "Framebuffer unsupported";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         return "Framebuffer incomplete (attachment)";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "Framebuffer incomplete (missing attachment)";
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:        return "Framebuffer incomplete (draw buffer)";
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:        return "Framebuffer incomplete (read buffer)";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:        return "Framebuffer incomplete (multisample)";
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:      return "Framebuffer incomplete (layer targets)";
        default:
            return "[UNKNOWN]";
        }
    }

    void FrameBuffer::bind(GLenum target) {
        // https://docs.gl/gl4/glBindFramebuffer
        glBindFramebuffer(target, m_Handle);
    }

    void FrameBuffer::unbind(GLenum target) {
        // https://docs.gl/gl4/glBindFramebuffer
        glBindFramebuffer(target, 0);
    }

    void FrameBuffer::draw_buffer(int color_attachment_idx) {
        assert(m_Handle);
        GLenum x = GL_COLOR_ATTACHMENT0 + color_attachment_idx; // needs to be a separate variable because it's referenced somewhere...

        // https://docs.gl/gl4/glDrawBuffer
        glNamedFramebufferDrawBuffer(m_Handle, x);
    }

    void FrameBuffer::blit_to_screen(
        GLint      src_left,
        GLint      src_top,
        GLint      src_right,
        GLint      src_bottom,
        GLint      dst_left,
        GLint      dst_top,
        GLint      dst_right,
        GLint      dst_bottom,
        GLbitfield mask,
        GLenum     filter
    ) {
        // https://docs.gl/gl4/glBlitFramebuffer
        glBlitNamedFramebuffer(
            m_Handle,
            0,
            src_left,
            src_top,
            src_right,
            src_bottom,
            dst_left,
            dst_top,
            dst_right,
            dst_bottom,
            mask,
            filter
        );
    }

    void FrameBuffer::blit_to_framebuffer(
        FrameBuffer& fb,
        GLint      src_left,
        GLint      src_top,
        GLint      src_right,
        GLint      src_bottom,
        GLint      dst_left,
        GLint      dst_top,
        GLint      dst_right,
        GLint      dst_bottom,
        GLbitfield mask,
        GLenum     filter
    ) {
        // https://docs.gl/gl4/glBlitFramebuffer
        glBlitNamedFramebuffer(
                m_Handle,
                fb.get_handle(),
                src_left,
                src_top,
                src_right,
                src_bottom,
                dst_left,
                dst_top,
                dst_right,
                dst_bottom,
                mask,
                filter
        );
    }
}