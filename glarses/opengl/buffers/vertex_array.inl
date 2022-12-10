#pragma once

#include "vertex_array.h"

namespace glarses::opengl::buffers {
	template <typename T>
	void VertexArray::bind_attribute(
		uint32_t         attribute,
		VertexBuffer<T>& buffer,
		e_Type           type,
		int              num_elements,
		size_t           stride,
		const void*      offset
	) {
		bind();
		buffer.bind();

		glEnableVertexAttribArray(attribute);

		glVertexAttribPointer(
			attribute,
			count,
			to_GLenum(type),
			GL_FALSE,			// normalized
			stride,
			offset
		);
	}
}