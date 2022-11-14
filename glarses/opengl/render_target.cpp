#include "render_target.h"

namespace glarses::opengl {
	RenderTarget::RenderTarget(int width, int height):
		m_Width(width),
		m_Height(height),
		m_DepthBuffer(
			width, 
			height, 
			true,   // depth_storage
			false   // stencil_storage
		)
	{
		m_ColorBuffer.init(width, height, GL_RGBA8);
		m_FrameBuffer.set_color_attachment(m_ColorBuffer);
		m_FrameBuffer.set_depth_attachment(m_DepthBuffer);
		m_FrameBuffer.draw_buffers(0);

		if (!m_FrameBuffer.is_ready())
			std::cerr << "Framebuffer not ready: " << m_FrameBuffer.get_error_message() << '\n';
	}

	struct RenderTarget::RenderScope {
		RenderScope(RenderTarget* target) {
			target->m_FrameBuffer.bind(GL_DRAW_FRAMEBUFFER);
			glViewport(0, 0, target->m_Width, target->m_Height);
		}

		~RenderScope() {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		RenderScope             (const RenderScope&) = delete;
		RenderScope& operator = (const RenderScope&) = delete;
		RenderScope             (RenderScope&&) noexcept = delete;
		RenderScope& operator = (RenderScope&&) noexcept = delete;
	};

	RenderTarget::RenderScope RenderTarget::render_scope() {
		return RenderScope(this);
	}
}