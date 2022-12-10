#include "shader_storage_buffer.h"

#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	ShaderStorageBuffer::~ShaderStorageBuffer() {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);
	}

	ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& ssb) noexcept :
		m_Handle      (ssb.m_Handle),
		m_BindingPoint(ssb.m_BindingPoint)
	{
		ssb.m_Handle       = 0;
		ssb.m_BindingPoint = 0;
	}

	ShaderStorageBuffer& ShaderStorageBuffer::operator = (ShaderStorageBuffer&& ssb) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		m_Handle       = ssb.m_Handle;
		m_BindingPoint = ssb.m_BindingPoint;

		ssb.m_Handle = 0;

		return *this;
	}

	ShaderStorageBuffer::ShaderStorageBuffer(e_BufferAccess access) {
		glGenBuffers(1, &m_Handle);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, to_GLenum(access));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_Handle);
	}

	uint32_t ShaderStorageBuffer::get_handle() const {
		return m_Handle;
	}

	uint32_t ShaderStorageBuffer::get_binding_point() const {
		return m_BindingPoint;
	}

	void ShaderStorageBuffer::bind(uint32_t binding_point) {
		m_BindingPoint = binding_point;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, m_Handle);
	}

	void ShaderStorageBuffer::unbind() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, 0);
	}

	void ShaderStorageBuffer::send_data(
		void*  data, 
		size_t num_bytes
	) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Handle);
		glBufferData(
			GL_SHADER_STORAGE_BUFFER,
			num_bytes,
			data,
			GL_DYNAMIC_DRAW
		);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

}