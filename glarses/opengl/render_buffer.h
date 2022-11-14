#pragma once

#include "../dependencies.h"

namespace glarses::opengl {
	class RenderBuffer {
	public:
		RenderBuffer() = default;
		RenderBuffer(
			GLsizei width,
			GLsizei height,
			bool    depth_storage,
			bool    stencil_storage
		);
		~RenderBuffer();

		RenderBuffer             (const RenderBuffer&) = delete;
		RenderBuffer& operator = (const RenderBuffer&) = delete;
		RenderBuffer             (RenderBuffer&& rb) noexcept;
		RenderBuffer& operator = (RenderBuffer&& rb) noexcept;

		GLuint get_handle() const;

		void bind(GLenum target);

	private:
		GLuint m_Handle = 0;
	};
}