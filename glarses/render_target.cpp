#include "render_target.h"

RenderTarget::~RenderTarget() {
	if (m_FBO)
		glDeleteFramebuffers(1, &m_FBO);

	if (m_Depth)
		glDeleteTextures(1, &m_Depth);

	if (m_Texture)
		glDeleteTextures(1, &m_Texture);
}

RenderTarget::RenderTarget(RenderTarget&& t) noexcept:
	m_Texture(t.m_Texture),
	m_Depth  (t.m_Depth),
	m_FBO    (t.m_FBO),
	m_Width  (t.m_Width),
	m_Height (t.m_Height)
{
	t.m_Texture = 0;
	t.m_Depth   = 0;
	t.m_FBO     = 0;
}

RenderTarget& RenderTarget::operator = (RenderTarget&& t) noexcept {
	if (m_FBO)
		glDeleteFramebuffers(1, &m_FBO);

	if (m_Depth)
		glDeleteTextures(1, &m_Depth);

	if (m_Texture)
		glDeleteTextures(1, &m_Texture);

	m_Texture = t.m_Texture;
	m_Depth   = t.m_Depth; 
	m_FBO     = t.m_FBO;
	m_Width   = t.m_Width;
	m_Height  = t.m_Height;

	t.m_Texture = 0;
	t.m_Depth   = 0;
	t.m_FBO     = 0;

	return *this;
}

RenderTarget RenderTarget::create(int width, int height) {
	GLuint tex   = 0;
	GLuint depth = 0;
	GLuint fbo   = 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D,    // target
		0,                // mipmap level
		GL_SRGB8_ALPHA8,  // internal format
		width,
		height,
		0,                // border
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		nullptr           // pixels
	);

	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D,         // target
		0,                     // mipmap level
		GL_DEPTH_COMPONENT32F, // internal format
		width,
		height,
		0,                     // border
		GL_DEPTH_COMPONENT,    // format
		GL_FLOAT,              // type,
		nullptr                // pixels
	);

	glGenFramebuffers(1, &fbo);

	RenderTarget result;
	result.m_Texture = tex;
	result.m_Depth   = depth;
	result.m_FBO     = fbo;
	result.m_Width   = width;
	result.m_Height  = height;

	return result;
}

GLuint RenderTarget::get_texture_handle() const {
	return m_Texture;
}

GLuint RenderTarget::get_depth_handle() const {
	return m_Depth;
}

GLuint RenderTarget::get_framebuffer_handle() const {
	return m_FBO;
}

int RenderTarget::get_width() const {
	return m_Width;
}

int RenderTarget::get_height() const {
	return m_Height;
}

void RenderTarget::bind_and_clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0); // bind mip level 0 to color attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, m_Depth,   0); // bind mip level 0 to depth 

	glViewport(0, 0, m_Width, m_Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_FRAMEBUFFER_SRGB);
}

void RenderTarget::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0); // bind null texture to color attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, 0, 0); // bind null texture to depth
}