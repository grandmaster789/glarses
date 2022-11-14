#include "texture.h"
#include <iostream>

Texture::~Texture() {
	if (m_Handle)
		glDeleteTextures(1, &m_Handle);
}

Texture::Texture(Texture&& t) noexcept:
	m_Handle(t.m_Handle),
	m_Width (t.m_Width),
	m_Height(t.m_Height)
{
	t.m_Handle = 0;
}

Texture& Texture::operator = (Texture&& t) noexcept {
	if (m_Handle)
		glDeleteTextures(1, &m_Handle);

	m_Handle = t.m_Handle;
	m_Width  = t.m_Width;
	m_Height = t.m_Height;

	t.m_Handle = 0;

	return *this;
}

void Texture::init(
	int    width,
	int    height,
	int    mip_levels,
	GLenum format,
	GLenum min_filter,
	GLenum mag_filter
) {
	if (m_Handle)
		throw std::runtime_error("Already initialized");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);                                  // https://docs.gl/gl4/glCreateTextures
	glTextureParameteri(m_Handle, GL_TEXTURE_MAX_LEVEL, mip_levels);
	glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, min_filter);
	glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTextureStorage2D(m_Handle, 1, format, width, height);                         // https://docs.gl/gl4/glTexStorage2D
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                                          // https://docs.gl/gl4/glPixelStore

	m_Width  = width;
	m_Height = height;
}

Texture Texture::load_file(const std::filesystem::path& p) {
	int w    = 0;
	int h    = 0;
	int comp = 0;

	std::cout << "Loading \"" << p.stem().string() << p.extension().string() << "\"\n";

	if (!std::filesystem::exists(p))
		throw std::runtime_error("Texture file does not exist");

	// TODO use unique_ptr wrapper here
	uint8_t* raw_data = stbi_load(
		p.string().c_str(), // source file
		&w,					// (out) image width
		&h,                 // (out) image height
		&comp,              // (out) image number of channels
		3                   // desired number of channels
	);

	// we now have enough data to fill out all of the openGL stuff
	Texture result;
	result.init(
		w,
		h,
		0,
		GL_RGB8,
		GL_LINEAR,
		GL_LINEAR
	);
	
	glTextureSubImage2D(
		result.m_Handle, 
		0, 
		0, 
		0, 
		w, 
		h, 
		GL_RGB, 
		GL_UNSIGNED_BYTE, 
		raw_data
	); // https://docs.gl/gl4/glTexSubImage2D
	
	glBindTextures(0, 1, &result.m_Handle);

	stbi_image_free(raw_data); // we should be done with the raw data at this point

	return result;
}

Texture Texture::load_raw_data(
	const std::vector<uint8_t>& buffer,
	int                         width,
	int                         height
) {
	Texture result;

	result.init(
		width,
		height,
		0,
		GL_RGB8,
		GL_LINEAR,
		GL_LINEAR
	);
 
	glTextureSubImage2D(
		result.m_Handle,   // target
		0,                 // mip level
		0,                 // x offset
		0,                 // y offset
		width, 
		height, 
		GL_RGB,            // format
		GL_UNSIGNED_BYTE,  // pixel datatype	
		buffer.data()
	); // https://docs.gl/gl4/glTexSubImage2D

	return result;
}

void Texture::bind(GLuint sampler_location) const {
	glBindTextures(sampler_location, 1, &m_Handle); // https://docs.gl/gl4/glBindTexture
}

GLuint Texture::get_handle() const {
	return m_Handle;
}

int Texture::get_width() const {
	return m_Width;
}

int Texture::get_height() const {
	return m_Height;
}