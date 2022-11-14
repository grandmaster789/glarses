#pragma once

#include "../dependencies.h"
#include "../math/rect.h"
#include "texture.h"
#include "render_buffer.h"

namespace glarses::opengl {
	class FrameBuffer {
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer             (const FrameBuffer&) = delete;
		FrameBuffer& operator = (const FrameBuffer&) = delete;
		FrameBuffer             (FrameBuffer&& fb) noexcept;
		FrameBuffer& operator = (FrameBuffer&& fb) noexcept;

		void bind(GLenum target);

		// maybe it would be good to *only* support renderbuffer attachments?
		void set_color_attachment(
			const Texture& tex, 
			int            mip_level      = 0, 
			int            attachment_idx = 0
		);

		void set_depth_attachment  (const RenderBuffer& buffer);
		void set_stencil_attachment(const RenderBuffer& buffer);

		void draw_buffers(int color_attachment_idx);

		void blit_to_screen(
			const math::Rect& source,
			const math::Rect& destination,
			GLbitfield        mask,
			GLenum            filter
		);

		GLenum      get_status()        const;
		bool        is_ready()          const;
		std::string get_error_message() const;

	private:
		GLuint m_Handle = 0;
	};
}