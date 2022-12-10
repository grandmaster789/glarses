#include "frame_buffer.h"
#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	FrameBuffer::~FrameBuffer() {
		if (m_Handle)
			glDeleteFramebuffers(1, &m_Handle);

		if (m_RenderTextureHandle)
			glDeleteTextures(1, &m_RenderTextureHandle);

		if (m_DepthStencilHandle)
			glDeleteRenderbuffers(1, &m_DepthStencilHandle);
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& fb) noexcept:
		m_Handle             (fb.m_Handle),
		m_RenderTextureHandle(fb.m_RenderTextureHandle),
		m_DepthStencilHandle (fb.m_DepthStencilHandle)
	{
		fb.m_Handle              = 0;
		fb.m_RenderTextureHandle = 0;
		fb.m_DepthStencilHandle  = 0;
	}

	FrameBuffer& FrameBuffer::operator = (FrameBuffer&& fb) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		if (m_RenderTextureHandle)
			glDeleteBuffers(1, &m_RenderTextureHandle);

		if (m_DepthStencilHandle)
			glDeleteBuffers(1, &m_DepthStencilHandle);

		m_Handle              = fb.m_Handle;
		m_RenderTextureHandle = fb.m_RenderTextureHandle;
		m_DepthStencilHandle  = fb.m_DepthStencilHandle;

		fb.m_Handle              = 0;
		fb.m_RenderTextureHandle = 0;
		fb.m_DepthStencilHandle  = 0;

		return *this;
	}

	FrameBuffer::FrameBuffer(
		size_t width,
		size_t height
	) {
		glGenFramebuffers (1, &m_Handle);
		glGenTextures     (1, &m_RenderTextureHandle);
		glGenRenderbuffers(1, &m_DepthStencilHandle);

		// this is a fullscreen texture, so we're picking the cheapest filter option as we're not expecting any advantage using anything else
		glBindTexture(GL_TEXTURE_2D, m_RenderTextureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		resize(width, height);
	}

	uint32_t FrameBuffer::get_handle() const {
		return m_Handle;
	}

	uint32_t FrameBuffer::get_texture_handle() const {
		return m_RenderTextureHandle;
	}

	uint32_t FrameBuffer::get_depthstencil_handle() const {
		return m_DepthStencilHandle;
	}

	void FrameBuffer::resize(
		size_t width,
		size_t height
	) {
		glBindTexture(GL_TEXTURE_2D, m_RenderTextureHandle);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,                // mip level
			GL_RGB,           // internal format
			width,
			height,
			0,                // border
			GL_RGB,           // format
			GL_UNSIGNED_BYTE, // channel type
			nullptr           // pixels
		);
		glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilHandle);
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			GL_DEPTH_STENCIL, // internal format
			width,
			height
		);
		glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind the depthstencil buffer

		// re-attach to the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilHandle);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilHandle);
		glFramebufferTexture(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			m_RenderTextureHandle,
			0                      // mip level
		);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind the framebuffer
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}