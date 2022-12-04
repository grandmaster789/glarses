#pragma once

#include "texture.h"
#include "render_buffer.h"
#include "frame_buffer.h"

namespace glarses::opengl {
	// 
	// 
	class RenderTarget {
	public:
		struct RenderScope;
		
		RenderTarget(int width, int height);

		[[nodiscard]]
		RenderScope render_scope();

		void blit_to_screen(int screen_width, int screen_height);

	private:
		int m_Width;
		int m_Height;

		Texture      m_ColorBuffer;
		RenderBuffer m_DepthBuffer;
		FrameBuffer  m_FrameBuffer;
	};
}