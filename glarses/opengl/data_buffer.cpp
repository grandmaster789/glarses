#include "data_buffer.h"

namespace glarses::opengl {
	DataBuffer::DataBuffer() {
		glCreateBuffers(1, &m_Handle);
	}

	DataBuffer::~DataBuffer() {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);
	}

	DataBuffer::DataBuffer(DataBuffer&& db) noexcept :
		m_Handle(db.m_Handle)
	{
		db.m_Handle = 0;
	}

	DataBuffer& DataBuffer::operator=(DataBuffer&& db) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		m_Handle = db.m_Handle;
		db.m_Handle = 0;

		return *this;
	}

	GLuint DataBuffer::get_handle() const {
		return m_Handle;
	}
}