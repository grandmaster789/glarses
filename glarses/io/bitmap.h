#pragma once

#include <iosfwd>
#include <vector>
#include <filesystem>

#include "../dependencies.h"

namespace glarses::io {
	enum class e_BitmapType {
		type_2d,
		type_cube // only very limited support for cube maps
	};

	// channel format
	enum class e_BitmapFormat {
		format_u8,
		format_f32
	};

	/*
	* Uncompressed raw image wrapper; intermediate between file formats and GPU-suitable textures
	*/
	class Bitmap {
	public:
		Bitmap() = default;

		// copies may be costly, so make those explicit via .clone()
		Bitmap             (const Bitmap&) = delete;
		Bitmap& operator = (const Bitmap&) = delete;
		Bitmap             (Bitmap&&) noexcept = default;
		Bitmap& operator = (Bitmap&&) noexcept = default;

		Bitmap(
			uint32_t       width, 
			uint32_t       height, 
			uint32_t       num_channels,
			e_BitmapFormat fmt,
			const void*    raw_source_data = nullptr
		);

		Bitmap(
			uint32_t       width,
			uint32_t       height,
			uint32_t       depth,
			uint32_t       num_channels,
			e_BitmapFormat fmt,
			const void*    raw_source_data = nullptr
		);

		[[nodiscard]] Bitmap clone() const;

		[[nodiscard]] uint32_t get_width()  const;
		[[nodiscard]] uint32_t get_height() const;
		[[nodiscard]] uint32_t get_depth()  const;

		[[nodiscard]] uint32_t       get_num_channels() const;
		[[nodiscard]] e_BitmapType   get_type()         const;
		[[nodiscard]] e_BitmapFormat get_format()       const;

		[[nodiscard]] const std::vector<uint8_t>& get_data() const;

		// pixel colors are converted from/to float as needed
		// NOTE these accessors are not suitable for subpixel operations;
		//      this class is mostly intended for raw I/O operations so it should be fine
		// NOTE color float range is assumed to be [0.0, 1.0]
		// NOTE extents are unchecked -- too much overhead
		void      set_pixel(int x, int y,        const glm::vec4& color);
		void      set_pixel(int x, int y, int z, const glm::vec4& color);

		[[nodiscard]] glm::vec4 get_pixel(int x, int y)        const;
		[[nodiscard]] glm::vec4 get_pixel(int x, int y, int z) const;

		friend std::ostream& operator << (std::ostream& os, const Bitmap& bmp);

		// for now, just use stbi for I/O file formats, (depth == 1) and type_2d
		//
		static Bitmap load(const std::filesystem::path& p);
		void          save(const std::filesystem::path& p);

	private:
		uint32_t m_Width         = 0;
		uint32_t m_Height        = 0;
		uint32_t m_Depth         = 0;
		
		// default to 2d r8g8b8 style
		uint32_t       m_NumChannels = 3;                         
		e_BitmapType   m_Type        = e_BitmapType::type_2d;
		e_BitmapFormat m_Format      = e_BitmapFormat::format_u8;

		// NOTE this is unpitched data; we may introduce row pitch for faster GPU manipulation at some point
		//      as an aside, manipulation methods typically use a matrix-y style interface
		std::vector<uint8_t> m_Data;
	};

	std::ostream& operator << (std::ostream& os, e_BitmapType   type);
	std::ostream& operator << (std::ostream& os, e_BitmapFormat fmt);
}