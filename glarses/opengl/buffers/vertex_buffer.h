#pragma once

#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	template <typename T>
	class VertexBuffer {
	public:
		VertexBuffer() = default;
		~VertexBuffer();

		VertexBuffer             (const VertexBuffer&) = delete;
		VertexBuffer& operator = (const VertexBuffer&) = delete;
		VertexBuffer             (VertexBuffer&& vb) noexcept;
		VertexBuffer& operator = (VertexBuffer&& vb) noexcept;

		VertexBuffer(
			T*     data, 
			size_t num_elements
		);

		VertexBuffer(
			const std::vector<T>& data
		);

		uint32_t get_handle() const;

		void bind();
		void unbind();

	private:
		uint32_t m_Handle = 0;
	};
}

#include "vertex_buffer.inl"