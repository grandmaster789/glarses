#pragma once

#include <cstdint>
#include <vector>

namespace glarses::opengl::buffers {
	class IndexBuffer {
	public:
		IndexBuffer() = default;
		~IndexBuffer();

		IndexBuffer             (const IndexBuffer&) = delete;
		IndexBuffer& operator = (const IndexBuffer&) = delete;
		IndexBuffer             (IndexBuffer&& ib) noexcept;
		IndexBuffer& operator = (IndexBuffer&& ib) noexcept;

		IndexBuffer(
			const uint32_t* raw_data, 
			size_t          num_elements
		);
		IndexBuffer(const std::vector<uint32_t>& data);

		uint32_t get_handle() const;

		void bind();
		void unbind();

	private:
		uint32_t m_Handle = 0;
	};
}