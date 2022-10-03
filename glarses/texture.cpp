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
	GLuint handle; 

	glCreateTextures(GL_TEXTURE_2D, 1, &handle);                                    // https://docs.gl/gl4/glCreateTextures
	glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(handle, 1, GL_RGB8, w, h);                                   // https://docs.gl/gl4/glTexStorage2D
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                                          // https://docs.gl/gl4/glPixelStore
	
	glTextureSubImage2D(handle, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, raw_data); // https://docs.gl/gl4/glTexSubImage2D
	
	glBindTextures(0, 1, &handle);

	stbi_image_free(raw_data); // we should be done with the raw data at this point

	// put the gl stuff into the Texture object
	Texture result;

	result.m_Handle = handle;
	result.m_Width  = w;
	result.m_Height = h;

	return result;
}

Texture Texture::load_raw_data(
	const std::vector<uint8_t>& buffer,
	int                         width,
	int                         height
) {
	GLuint handle;
	glCreateTextures(GL_TEXTURE_2D, 1, &handle);                                                  // https://docs.gl/gl4/glCreateTextures
	glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexStorage2D(handle, 1, GL_RGB8, width, height);                                            // https://docs.gl/gl4/glTexStorage2D
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                                                        // https://docs.gl/gl4/glPixelStore
	 
	glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data()); // https://docs.gl/gl4/glTexSubImage2D

	// put the gl stuff into the Texture object
	Texture result;

	result.m_Handle = handle;

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