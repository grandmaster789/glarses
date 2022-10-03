#pragma once

#include "dependencies.h"

// Texture suitable to become a render target (includes a depthbuffer + framebuffer object)
//
class RenderTarget {
public:
	RenderTarget() = default;
	~RenderTarget();

	RenderTarget             (const RenderTarget&) = delete;
	RenderTarget& operator = (const RenderTarget&) = delete;
	RenderTarget             (RenderTarget&& t) noexcept; 
	RenderTarget& operator = (RenderTarget&& t) noexcept;

	static RenderTarget create(int width, int height);

	GLuint get_texture_handle()     const; // sRGBA
	GLuint get_depth_handle()       const; // 32F
	GLuint get_framebuffer_handle() const; 

	int get_width() const;
	int get_height() const;

	void bind_and_clear();
	void unbind();

private:
	GLuint m_Texture = 0;
	GLuint m_Depth   = 0;
	GLuint m_FBO     = 0; // framebuffer object
	int m_Width      = 0;
	int m_Height     = 0;
};