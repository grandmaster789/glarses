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

		RenderScope render_scope();

	private:
		int m_Width;
		int m_Height;

		Texture      m_ColorBuffer;
		RenderBuffer m_DepthBuffer;
		FrameBuffer  m_FrameBuffer;
	};
}