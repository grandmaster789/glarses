#include "render_buffer.h"

namespace glarses::opengl {
	RenderBuffer::RenderBuffer(
		GLsizei width,
		GLsizei height,
		bool depth_storage,
		bool stencil_storage
	) {
		glCreateRenderbuffers(1, &m_Handle);

		if (depth_storage)
			glNamedRenderbufferStorage(m_Handle, GL_DEPTH_COMPONENT, width, height);
		if (stencil_storage)
			glNamedRenderbufferStorage(m_Handle, GL_STENCIL_COMPONENTS, width, height);
		
		// color storage?
			
	}

	RenderBuffer::~RenderBuffer() {
		if (m_Handle)
			glDeleteRenderbuffers(1, &m_Handle);
	}

	RenderBuffer::RenderBuffer(RenderBuffer&& rb) noexcept:
		m_Handle(rb.m_Handle)
	{
		rb.m_Handle = 0;
	}

	RenderBuffer& RenderBuffer::operator = (RenderBuffer&& rb) noexcept {
		std::swap(rb.m_Handle, m_Handle);
		return *this;
	}

	GLuint RenderBuffer::get_handle() const {
		return m_Handle;
	}

	void RenderBuffer::bind(GLenum target) {
		glBindRenderbuffer(target, m_Handle);
	}
}