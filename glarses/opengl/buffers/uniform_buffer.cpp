#include "uniform_buffer.h"
#include "../../dependencies.h"
#include "../resources/shader.h"

namespace glarses::opengl::buffers {
	UniformBuffer::~UniformBuffer() {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);
	}

	UniformBuffer::UniformBuffer(
		size_t         num_bytes,
		uint32_t       binding_point,
		uint32_t       offset,
		e_BufferAccess access
	) {
		glGenBuffers(1, &m_Handle);

		bind();
		
		glBufferData(
			GL_UNIFORM_BUFFER,
			num_bytes,
			nullptr, // data
			to_GLenum(access)
		);

		unbind();

		glBindBufferRange(
			GL_UNIFORM_BUFFER,
			binding_point,
			m_Handle,
			offset,
			num_bytes
		);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& ub) noexcept:
		m_Handle(ub.m_Handle)
	{
		ub.m_Handle = 0;
	}

	UniformBuffer& UniformBuffer::operator = (UniformBuffer&& ub) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		m_Handle = ub.m_Handle;

		ub.m_Handle = 0;

		return *this;
	}

	uint32_t UniformBuffer::get_handle() const {
		return m_Handle;
	}

	void UniformBuffer::bind() {
		glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
	}

	void UniformBuffer::unbind() {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::bind_to_shader(
		resources::Shader& shader,
		uint32_t           location,
		uint32_t           binding_point
	) {
		glUniformBlockBinding(
			shader.get_handle(),
			location,
			binding_point
		);
	}

	void UniformBuffer::bind_to_shader(
		resources::Shader& shader,
		const std::string& location_name,
		uint32_t           binding_point
	) {
		bind_to_shader(
			shader,
			get_location(shader, location_name),
			binding_point
		);
	}

	uint32_t UniformBuffer::get_location(
		const resources::Shader& shader,
		const std::string& location_name
	) {
		return glGetUniformBlockIndex(
			shader.get_handle(),
			location_name.c_str()
		);
	}
}