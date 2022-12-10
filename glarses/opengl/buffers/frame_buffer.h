#pragma once

#include <cstdint>

namespace glarses::opengl::buffers {
	class FrameBuffer {
	public:
		FrameBuffer() = default;
		~FrameBuffer();

		FrameBuffer             (const FrameBuffer&) = delete;
		FrameBuffer& operator = (const FrameBuffer&) = delete;
		FrameBuffer             (FrameBuffer&& fb) noexcept;
		FrameBuffer& operator = (FrameBuffer&& fb) noexcept;

		FrameBuffer(
			size_t width,
			size_t heigth
		);

		uint32_t get_handle() const;
		uint32_t get_texture_handle() const;
		uint32_t get_depthstencil_handle() const;

		void resize(
			size_t width, 
			size_t height
		);

		void bind();
		void unbind();

	private:
		uint32_t m_Handle              = 0; // FBO
		uint32_t m_RenderTextureHandle = 0; // RGB
		uint32_t m_DepthStencilHandle  = 0; // 24 bits depth, 8 bits stencil
	};
}