#pragma once

#include <filesystem>
#include <vector>
#include <cstdint>

#include "../dependencies.h"

// very basic wrapper; should cover just the most common use case
// (always loads as RGB8 texture, no mipmaps)
// -- not particularily efficient or flexible, but easy to use
// -- should probably add bindless methods here
//
namespace glarses::opengl {
	class Texture {
	public:
		Texture() = default;
		~Texture();

		Texture             (const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;
		Texture             (Texture&& t) noexcept;
		Texture& operator = (Texture&& t) noexcept;

		// NOTE the defaults reflect what's suitable for rendertarget textures
		void init(
			int    width,
			int    height,
			int    mip_levels     = 0,
			GLenum storage_format = GL_RGBA8,
			GLenum min_filter     = GL_NEAREST,
			GLenum mag_filter     = GL_NEAREST
		);

		// uses STB to load the file - supports [jpg, png, bmp] at least
		static Texture load_file(const std::filesystem::path& p);
		static Texture load_raw_data(
			const std::vector<uint8_t>& buffer, // assuming RGB8 format, no mip levels
			int                         width,
			int                         height
		);

		void bind(GLuint sampler_location) const;

		GLuint get_handle() const;

		int get_width() const;
		int get_height() const;

	private:
		GLuint m_Handle = 0;

		int    m_Width  = 0;
		int    m_Height = 0;
	};
}