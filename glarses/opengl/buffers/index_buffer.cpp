#include "index_buffer.h"
#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	IndexBuffer::~IndexBuffer() {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& ib) noexcept:
		m_Handle(ib.m_Handle)
	{
		ib.m_Handle = 0;
	}

	IndexBuffer& IndexBuffer::operator = (IndexBuffer&& ib) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		m_Handle    = ib.m_Handle;

		ib.m_Handle = 0;

		return *this;
	}

	IndexBuffer::IndexBuffer(
		const uint32_t* raw_data, 
		size_t          num_elements
	) {
		glGenBuffers(1, &m_Handle);
		
		bind();
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			num_elements * sizeof(uint32_t), // size in bytes
			raw_data,
			GL_STATIC_DRAW                   // usage
		);
		unbind();
	}

	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& data):
		IndexBuffer(data.data(), data.size()) // defer to the other constructor
	{
	}

	uint32_t IndexBuffer::get_handle() const {
		return m_Handle;
	}

	void IndexBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}