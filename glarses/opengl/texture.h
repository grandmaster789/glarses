#pragma once

#include "../dependencies.h"
#include <filesystem>
#include <iosfwd>

namespace glarses::opengl {
	enum class e_TextureType {
		unknown      = 0,
		texture_2d   = GL_TEXTURE_2D,
		texture_cube = GL_TEXTURE_CUBE_MAP,
	};

	// texture formats supported:
	//   KTX                                               (via gli)
	//   jpeg, png, bmp, hdr, psd, tga, gif, pic, pgm, ppm (via stbi)

	class Texture {
	public:
		Texture() = default;
		~Texture();

		Texture             (const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;
		Texture             (Texture&& t) noexcept;
		Texture& operator = (Texture&& t) noexcept;

		Texture(
			e_TextureType                type,
			const std::filesystem::path& path,
			GLenum                       wrap_type = GL_REPEAT
		);

		Texture(
			GLenum   type, 
			uint32_t width, 
			uint32_t height, 
			GLenum   internal_format
		);

        // if no raw data is provided, a checkerboard pattern is generated
		Texture(
			uint32_t width, 
			uint32_t height, 
			void*    raw_data
		);

		e_TextureType get_type() const;
		uint32_t      get_handle() const;
		uint64_t      get_bindless_handle() const;

		friend std::ostream& operator << (std::ostream&  os, const Texture& t);

	private:
		e_TextureType m_Type           = e_TextureType::unknown;
		uint32_t      m_Handle         = 0;
		uint64_t      m_BindlessHandle = 0;
	};

	std::ostream& operator << (std::ostream& os, e_TextureType type);
}