#pragma once

#include "vertex_buffer.h"

namespace glarses::opengl::buffers {
	template <typename T>
	VertexBuffer<T>::~VertexBuffer() {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);
	}

	template <typename T>
	VertexBuffer<T>::VertexBuffer(VertexBuffer&& vb) noexcept:
		m_Handle(vb.m_Handle)
	{
		vb.m_Handle = 0;
	}

	template <typename T>
	VertexBuffer<T>& VertexBuffer<T>::operator = (VertexBuffer&& vb) noexcept {
		if (m_Handle)
			glDeleteBuffers(1, &m_Handle);

		m_Handle = vb.m_Handle;

		vb.m_Handle = 0;

		return *this;
	}

	template <typename T>
	VertexBuffer<T>::VertexBuffer(
		T*     data,
		size_t num_elements
	) {
		glGenBuffers(1, &m_Handle);
		
		bind();

		glBufferData(
			GL_ARRAY_BUFFER,
			num_elements * sizeof(T), // assume packed data
			data,
			GL_STATIC_DRAW
		);

		unbind();
	}

	template <typename T>
	VertexBuffer<T>::VertexBuffer(
		const std::vector<T>& data
	):
		VertexBuffer(data.data(), data.size())
	{
	}

	template <typename T>
	uint32_t VertexBuffer<T>::get_handle() const {
		return m_Handle;
	}

	template <typename T>
	void VertexBuffer<T>::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	}

	template <typename T>
	void VertexBuffer<T>::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}