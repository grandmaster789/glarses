#include "vertex.h"
#include "data_buffer.h"

namespace glarses::opengl {
	const VertexMember Vertex::s_PositionDesc = {
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		offsetof(Vertex, m_Position)
	};

	const VertexMember Vertex::s_ColorDesc = {
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex), 
		offsetof(Vertex, m_Color) 
	};

	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray() {
		if (m_Handle)
			glDeleteVertexArrays(1, &m_Handle);
	}

	VertexArray::VertexArray(VertexArray&& va) noexcept :
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

	GLuint VertexArray::get_handle() const {
		return m_Handle;
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_Handle);
	}

	void VertexArray::attach(
		const DataBuffer&   db,
		GLuint              index,
		const VertexMember& desc
	) {
		glEnableVertexArrayAttrib(
			m_Handle, 
			index
		);

		glVertexArrayVertexBuffer(
			m_Handle,        // vaobj
			0,               // binding index
			db.get_handle(), // buffer
			0,               // offset
			desc.m_Stride    // stride
		);

		glVertexArrayAttribBinding(
			m_Handle,
			index, 
			0
		);

		glVertexArrayAttribFormat(
			m_Handle, 
			index, 
			desc.m_Size, 
			desc.m_Type, 
			desc.m_IsNormalized, 
			desc.m_Offset
		);
	}
}