#include "uniform_buffer.h"

namespace glarses::opengl {
	UniformBuffer::~UniformBuffer() {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& u) noexcept :
		m_Handle      (u.m_Handle),
		m_BindingPoint(u.m_BindingPoint),
		m_BufferSize  (u.m_BufferSize)
	{
		u.m_Handle = 0;
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& u) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		m_Handle       = u.m_Handle;
		m_BindingPoint = u.m_BindingPoint;
		m_BufferSize   = u.m_BufferSize;

		u.m_Handle = 0;

		return *this;
	}

	void UniformBuffer::bind() {
		// https://docs.gl/gl4/glBindBufferRange
		glBindBufferRange(
			GL_UNIFORM_BUFFER, 
			m_BindingPoint, 
			m_Handle, 
			0, 
			m_BufferSize
		);
	}
}