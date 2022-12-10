#pragma once

#include "vertex_buffer.h"
#include "data_type.h"
#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray             (const VertexArray&) = delete;
		VertexArray& operator = (const VertexArray&) = delete;
		VertexArray             (VertexArray&& va) noexcept;
		VertexArray& operator = (VertexArray&& va) noexcept;

		uint32_t get_handle() const;

		void bind();
		void unbind();

		// NOTE this leaves the VAO in a bound state
		template <typename T>
		void bind_attribute(
			uint32_t         attribute,
			VertexBuffer<T>& buffer,
			e_DataType       type,
			int              num_elements, // must be 1/2/3/4
			size_t           stride,
			const void*      offset
		);

	private:
		uint32_t m_Handle;
	};
}