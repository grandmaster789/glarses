#pragma once

#include "uniform_buffer.h"
#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	template <typename T>
	void UniformBuffer::set_subdata(
		const T& data,
		size_t   offset
	) {
		bind();

		glBufferSubData(
			GL_UNIFORM_BUFFER,
			offset,
			sizeof(T),
			std::addressof(data)
		);

		unbind();
	}

	template <typename T>
	void UniformBuffer::set_subdata(
		const T&                       data,
		std::reference_wrapper<size_t> offset_inout
	) {
		bind();

		size_t num_bytes = sizeof(T);

		glBufferSubData(
			GL_UNIFORM_BUFFER,
			offset_inout.get(),
			num_bytes,
			std::addressof(data)
		);

		offset_inout.get() += num_bytes;

		unbind();
	}
}