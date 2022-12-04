#include "uniform_buffer.h"

namespace glarses::opengl {
	template <typename T>
	UniformBuffer UniformBuffer::create(GLuint binding_point) {
		GLuint     handle      = 0;
		GLsizeiptr buffer_size = sizeof(T);

		glCreateBuffers(1, &handle);

		// https://docs.gl/gl4/glBufferStorage
		glNamedBufferStorage(
			handle,
			buffer_size,
			nullptr,               // initial data
			GL_DYNAMIC_STORAGE_BIT // data content changes every frame
		);

		// https://docs.gl/gl4/glBindBufferRange
		glBindBufferRange(
			GL_UNIFORM_BUFFER,
			binding_point,
			handle,
			0,
			buffer_size
		); 

		UniformBuffer result;

		result.m_Handle       = handle;
		result.m_BindingPoint = binding_point;
		result.m_BufferSize   = buffer_size;

		return result;
	}

	template <typename T>
	void UniformBuffer::set_data(const T& data) {
		// https://docs.gl/gl4/glBufferSubData
		glNamedBufferSubData(
			m_Handle,
			0,
			m_BufferSize,
			data
		);
	}
}