#include "frame_buffer.h"

namespace glarses::opengl {
	FrameBuffer::FrameBuffer() {
		glCreateFramebuffers(1, &m_Handle);
	}

	FrameBuffer::~FrameBuffer() {
		if (m_Handle)
			glDeleteFramebuffers(1, &m_Handle);
	}
	
	FrameBuffer::FrameBuffer(FrameBuffer&& fb) noexcept:
		m_Handle(fb.m_Handle)
	{
		fb.m_Handle = 0;
	}

	FrameBuffer& FrameBuffer::operator = (FrameBuffer&& fb) noexcept {
		if (m_Handle)
			glDeleteFramebuffers(1, &m_Handle);

		m_Handle = fb.m_Handle;

		fb.m_Handle = 0;

		return *this;
	}

	void FrameBuffer::bind(GLenum target) {
		glBindFramebuffer(target, m_Handle);
	}

	void FrameBuffer::set_color_attachment(
		const Texture& tex,
		int            mip_level,
		int            attachment_idx
	) {
		// NOTE should really check that everything is valid
		glNamedFramebufferTexture(
			m_Handle, 
			attachment_idx + GL_COLOR_ATTACHMENT0, 
			tex.get_handle(), 
			mip_level
		);
	}

	void FrameBuffer::set_depth_attachment(const RenderBuffer& buffer) {
		glNamedFramebufferRenderbuffer(
			m_Handle,
			GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER,
			buffer.get_handle()
		);
	}

	void FrameBuffer::set_stencil_attachment(const RenderBuffer& buffer) {
		glNamedFramebufferRenderbuffer(
			m_Handle,
			GL_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER,
			buffer.get_handle()
		);
	}

	void FrameBuffer::draw_buffers(int color_attachment_idx) {
		// NOTE should really check that the attachment is valid
		GLenum idx = GL_COLOR_ATTACHMENT0 + color_attachment_idx;

		glNamedFramebufferDrawBuffers(m_Handle, 1, &idx);
	}

	void FrameBuffer::blit_to_screen(
		const math::Rect& source,
		const math::Rect& destination,
		GLbitfield        mask,
		GLenum            filter
	) {
		glBlitNamedFramebuffer(
			m_Handle, // readFramebuffer
			0,        // drawFramebuffer (0 is the screen)
			source.m_Left,
			source.m_Top,
			source.m_Right,
			source.m_Bottom,
			destination.m_Left,
			destination.m_Top,
			destination.m_Right,
			destination.m_Bottom,
			mask,
			filter
		);
	}

	GLenum FrameBuffer::get_status() const {
		return glCheckNamedFramebufferStatus(m_Handle, GL_FRAMEBUFFER);
	}

	bool FrameBuffer::is_ready() const {
		return (get_status() == GL_FRAMEBUFFER_COMPLETE);
	}

	std::string FrameBuffer::get_error_message() const {
		switch (get_status()) {					    
		case GL_FRAMEBUFFER_COMPLETE:                      return "Framebuffer complete";                        break;
		case GL_FRAMEBUFFER_UNDEFINED:                     return "Framebuffer undefined";                       break;
		case GL_FRAMEBUFFER_UNSUPPORTED:                   return "Framebuffer unsupported";                     break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         return "Framebuffer incomplete - attachment";         break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "Framebuffer incomplete - missing attachment"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:        return "Framebuffer incomplete - draw buffer";        break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:        return "Framebuffer incomplete - read buffer";        break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:        return "Framebuffer incomplete - multisample";        break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:      return "Framebuffer incomplete - layer targets";      break;
		default:
			return "[Unknown]";
		}
	}
}