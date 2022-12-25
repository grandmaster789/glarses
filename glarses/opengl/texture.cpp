#include "texture.h"

#include <ostream>

namespace {
	uint32_t count_mip_levels(
		uint32_t width,
		uint32_t height
	) {
		uint32_t levels = 1;

		while ((width | height) >> levels)
			++levels;

		return levels;
	}

	uint8_t* generate_checkerboard(
		int* out_width,
		int* out_height
	) {
		int width       = 128;
		int height      = 128;
		int square_size = 8;

		// pretend we're stbi and use malloc to allocate
		uint8_t* result = static_cast<uint8_t*>(malloc(width * height * 4));
		if (!result)
			throw std::runtime_error("Out of memory");

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				bool white = false;

				if (((y / square_size) % 2) && !((x / square_size) % 2))
					white = true;

				if (!((y / square_size) % 2) && ((x / square_size) % 2))
					white = true;

				if (white) {
					result[y * width * 4 + x * 4 + 0] = 0xFF;
					result[y * width * 4 + x * 4 + 1] = 0xFF;
					result[y * width * 4 + x * 4 + 2] = 0xFF;
					result[y * width * 4 + x * 4 + 3] = 0xFF;
				}
				else {
					result[y * width * 4 + x * 4 + 0] = 0x00;
					result[y * width * 4 + x * 4 + 1] = 0x00;
					result[y * width * 4 + x * 4 + 2] = 0x00;
					result[y * width * 4 + x * 4 + 3] = 0xFF;
				}
			}
		}

		if (out_width)
			*out_width = width;
		if (out_height)
			*out_height = height;

		return result;
	}
}

namespace glarses::opengl {
	Texture::~Texture() {
		if (m_BindlessHandle)
			glMakeTextureHandleNonResidentARB(m_BindlessHandle);

		if (m_Handle)
			glDeleteTextures(1, &m_Handle);
	}

	Texture::Texture(Texture&& t) noexcept:
		m_Type          (t.m_Type),
		m_Handle        (t.m_Handle),
		m_BindlessHandle(t.m_BindlessHandle)
	{
		t.m_Handle         = 0;
		t.m_BindlessHandle = 0;
	}

	Texture& Texture::operator = (Texture&& t) noexcept {
		if (m_BindlessHandle)
			glMakeTextureHandleNonResidentARB(m_BindlessHandle);

		if (m_Handle)
			glDeleteTextures(1, &m_Handle);

		m_Type           = t.m_Type;
		m_Handle         = t.m_Handle;
		m_BindlessHandle = t.m_BindlessHandle;

		t.m_Handle         = 0;
		t.m_BindlessHandle = 0;

		return *this;
	}

	Texture::Texture(
		e_TextureType                type, 
		const std::filesystem::path& path, 
		GLenum                       wrap_type
	):
		m_Type(type)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // assume packed, unpitched input data

		glCreateTextures(
			static_cast<GLenum>(type), // type
			1,                         // count
			&m_Handle                  // (out) result
		);

		glTextureParameteri(m_Handle, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, wrap_type);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, wrap_type);

		switch (type) {
		case e_TextureType::texture_2d:
		{
			int width  = 0;
			int height = 0;

			uint8_t* raw_data = stbi_load(
				path.generic_string().c_str(),
				&width,
				&height,
				nullptr,       // (out) number of channels in file
				STBI_rgb_alpha // let stb convert to 4-channel if needed
			);

			if (!raw_data) {
				// report an error, provide a fallback checkerboard pattern
				std::cerr << "Failed to load " << path.generic_string() << ", falling back to checkerboard\n";

				raw_data = generate_checkerboard(
					&width,
					&height
				);
			}

			uint32_t num_mips = count_mip_levels(width, height);

			glTextureStorage2D(
				m_Handle,
				num_mips,
				GL_RGBA8,
				width,
				height
			);

			glTextureSubImage2D(
				m_Handle,
				0,                 // mip level
				0,                 // x offset
				0,                 // y offset
				width,
				height,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				raw_data
			);

			stbi_image_free(raw_data);

			glGenerateTextureMipmap(m_Handle);

			glTextureParameteri(m_Handle, GL_TEXTURE_MAX_LEVEL, num_mips - 1);
			glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_Handle, GL_MAX_TEXTURE_MAX_ANISOTROPY, 16);
		}
			break;

		case e_TextureType::texture_cube:
			throw std::runtime_error("TODO");
			break;

		default:
			throw std::runtime_error("Unsupported texture type");
		}

		// make the texture resident
		m_BindlessHandle = glGetTextureHandleARB(m_Handle);
		glMakeTextureHandleResidentARB(m_BindlessHandle);
	}

	Texture::Texture(
		GLenum   type, 
		uint32_t width, 
		uint32_t height, 
		GLenum   internal_format
	) {
		glCreateTextures(
			type,          // type
			1,             // count
			&m_Handle      // (out) result
		);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // assume packed, unpitched input data

		glTextureParameteri(m_Handle, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(
			m_Handle, 
			count_mip_levels(width, height), 
			internal_format, 
			width, 
			height
		);
	}

	Texture::Texture(
		uint32_t width, 
		uint32_t height, 
		void*    raw_data
	):
		m_Type(e_TextureType::texture_2d)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // assume packed, unpitched input data

		glCreateTextures(
			GL_TEXTURE_2D, // type
			1,             // count
			&m_Handle      // (out) result
		);

		uint32_t mips = count_mip_levels(width, height);

		glTextureStorage2D(
			m_Handle, 
			mips, 
			GL_RGBA8, 
			width, 
			height
		);

		glTextureSubImage2D(
			m_Handle, 
			0, 
			0, 
			0, 
			width, 
			height,
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			raw_data
		);

		glGenerateTextureMipmap(m_Handle);

		glTextureParameteri(m_Handle, GL_TEXTURE_MAX_LEVEL, mips - 1);
		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAX_ANISOTROPY, 16);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

		m_BindlessHandle = glGetTextureHandleARB(m_Handle);

		glMakeTextureHandleResidentARB(m_BindlessHandle);
	}

	e_TextureType Texture::get_type() const {
		return m_Type;
	}

	uint32_t Texture::get_handle() const {
		return m_Handle;
	}

	uint64_t Texture::get_bindless_handle() const {
		return m_BindlessHandle;
	}

	std::ostream& operator << (std::ostream& os, const Texture& t) {
		os << "Texture(" << t.m_Type << ")";

		return os;
	}

	std::ostream& operator << (std::ostream& os, e_TextureType type) {
		switch (type) {
		case e_TextureType::unknown:      os << "unknown";      break;
		case e_TextureType::texture_2d:   os << "texture_2d";   break;
		case e_TextureType::texture_cube: os << "texture_cube"; break;
		default:
			os << "[UNKNOWN]";
		}

		return os;
	}
}