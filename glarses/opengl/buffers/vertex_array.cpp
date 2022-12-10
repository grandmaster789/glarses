#include "vertex_array.h"

namespace glarses::opengl::buffers {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray() {
		if (m_Handle)
			glDeleteVertexArrays(1, &m_Handle);
	}

	VertexArray::VertexArray(VertexArray&& va) noexcept:
		m_Handle(va.m_Handle)
	{
		va.m_Handle = 0;
	}

	VertexArray& VertexArray::operator = (VertexArray&& va) noexcept {
		if (m_Handle)
			glDeleteVertexArrays(1, &m_Handle);

		m_Handle = va.m_Handle;

		va.m_Handle = 0;

		return *this;
	}
	
	uint32_t VertexArray::get_handle() const {
		return m_Handle;
	}

	void VertexArray::bind() {
		glBindVertexArray(m_Handle);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}
}