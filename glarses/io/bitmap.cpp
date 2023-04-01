#include "bitmap.h"

#include <ostream>

namespace glarses::io {
	namespace {
		size_t bytes_per_component(e_BitmapFormat fmt) {
			switch (fmt) {
			case e_BitmapFormat::format_u8:  return 1;
			case e_BitmapFormat::format_f32: return 4;
			}

			throw std::runtime_error("Unsupported bitmap channel format");
		}
	}

	Bitmap::Bitmap(
		uint32_t       width,
		uint32_t       height,
		uint32_t       num_channels,
		e_BitmapFormat fmt,
		const void*    raw_source_data
	):
		m_Width      (width),
		m_Height     (height),
		m_Depth      (1),
		m_NumChannels(num_channels),
		m_Format     (fmt)
	{
		size_t num_bytes = width * height * num_channels * bytes_per_component(fmt);

		m_Data.resize(num_bytes);

		if (raw_source_data)
			memcpy(m_Data.data(), raw_source_data, num_bytes);
	}

	Bitmap::Bitmap(
		uint32_t       width,
		uint32_t       height,
		uint32_t       depth,
		uint32_t       num_channels,
		e_BitmapFormat fmt,
		const void*    raw_source_data
	):
		m_Width      (width),
		m_Height     (height),
		m_Depth      (depth),
		m_NumChannels(num_channels),
		m_Format     (fmt)
	{
		size_t num_bytes = width * height * depth * num_channels * bytes_per_component(fmt);

		m_Data.resize(num_bytes);

		if (raw_source_data)
			memcpy(m_Data.data(), raw_source_data, num_bytes);
	}

	Bitmap Bitmap::clone() const {
		Bitmap result;

		result.m_Width       = m_Width;
		result.m_Height      = m_Height;
		result.m_Depth       = m_Depth;

		result.m_NumChannels = m_NumChannels;
		result.m_Type        = m_Type;
		result.m_Format      = m_Format;

		result.m_Data        = m_Data;

		return result;
	}

	uint32_t Bitmap::get_width() const {
		return m_Width;
	}

	uint32_t Bitmap::get_height() const {
		return m_Height;
	}

	uint32_t Bitmap::get_depth() const {
		return m_Depth;
	}

	uint32_t Bitmap::get_num_channels() const {
		return m_NumChannels;
	}

	e_BitmapType Bitmap::get_type() const {
		return m_Type;
	}

	e_BitmapFormat Bitmap::get_format() const {
		return m_Format;
	}

	const std::vector<uint8_t>& Bitmap::get_data() const {
		return m_Data;
	}

	void Bitmap::set_pixel(int x_, int y_, const glm::vec4& color) {
		auto x = static_cast<uint32_t>(x_);
		auto y = static_cast<uint32_t>(y_);

		uint32_t y_stride = m_NumChannels * m_Width;
		uint32_t x_stride = m_NumChannels;

		switch (m_Format) {
		case e_BitmapFormat::format_u8:
			// rescale each channel from [0.0f; 1.0f] to [0x00; 0xFF]

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				m_Data[
					y * y_stride +
					x * x_stride +
					i
				] = static_cast<uint8_t>(color[i] * 255.0f);

					break;

		case e_BitmapFormat::format_f32:
		{
			auto* data = reinterpret_cast<float*>(m_Data.data());

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				data[
					y * y_stride +
					x * x_stride +
					i
				] = color[i];
		}
		break;

		default:
			throw std::runtime_error("Unsupported bitmap channel format");
		}
	}

	void Bitmap::set_pixel(int x_, int y_, int z_, const glm::vec4& color) {
		auto x = static_cast<uint32_t>(x_);
        auto y = static_cast<uint32_t>(y_);
        auto z = static_cast<uint32_t>(z_);

		uint32_t z_stride = m_NumChannels * m_Width * m_Height;
		uint32_t y_stride = m_NumChannels * m_Width;
		uint32_t x_stride = m_NumChannels;

		switch (m_Format) {
		case e_BitmapFormat::format_u8:
			// rescale each channel from [0.0f; 1.0f] to [0x00; 0xFF]

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				m_Data[
					z * z_stride + 
					y * y_stride +
					x * x_stride + 
					i
				] = static_cast<uint8_t>(color[i] * 255.0f);

			break;

		case e_BitmapFormat::format_f32:
		{
            auto* data = reinterpret_cast<float*>(m_Data.data());

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				data[
					z * z_stride + 
					y * y_stride + 
					x * x_stride + 
					i
				] = color[i];
		}
		break;

		default:
			throw std::runtime_error("Unsupported bitmap channel format");
		}
	}

	glm::vec4 Bitmap::get_pixel(int x_, int y_) const {
		glm::vec4 result(0, 0, 0, 0);

        auto x = static_cast<uint32_t>(x_);
        auto y = static_cast<uint32_t>(y_);

		uint32_t y_stride = m_NumChannels * m_Width;
		uint32_t x_stride = m_NumChannels;

		switch (m_Format) {
		case e_BitmapFormat::format_u8: 
			// rescale from [0x00;0xFF] to [0.0f; 1.0f]
			
			for (uint32_t i = 0; i < m_NumChannels; ++i)
				result[i] = static_cast<float>(m_Data[x_stride * x + y_stride * y]) / 255.0f;

			break;

		case e_BitmapFormat::format_f32:
		{
			const float* data = reinterpret_cast<const float*>(m_Data.data());

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				result[i] = data[x_stride * x + y_stride * y];
		}

			break;

		default:
			throw std::runtime_error("Unsupported bitmap channel format");
		}

		return result;
	}

	glm::vec4 Bitmap::get_pixel(int x_, int y_, int z_) const {
		glm::vec4 result(0, 0, 0, 0);

        auto x = static_cast<uint32_t>(x_);
        auto y = static_cast<uint32_t>(y_);
        auto z = static_cast<uint32_t>(z_);

		uint32_t z_stride = m_NumChannels * m_Width * m_Height;
		uint32_t y_stride = m_NumChannels * m_Width;
		uint32_t x_stride = m_NumChannels;

		switch (m_Format) {
		case e_BitmapFormat::format_u8:
			// rescale from [0x00; 0xFF] to [0.0f; 1.0f]

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				result[i] = static_cast<float>(m_Data[x_stride * x + y_stride * y + z_stride * z]) / 255.0f;

			break;

		case e_BitmapFormat::format_f32:
		{
			const float* data = reinterpret_cast<const float*>(m_Data.data());

			for (uint32_t i = 0; i < m_NumChannels; ++i)
				result[i] = data[x_stride * x + y_stride * y + z_stride * z];
		}

			break;

		default:
			throw std::runtime_error("Unsupported bitmap channel format");
		}

		return result;
	}

	Bitmap Bitmap::load(const std::filesystem::path& p) {
		if (!std::filesystem::exists(p))
			throw std::runtime_error("File does not exist");

		int width;
		int height;
		int num_channels;

		auto raw_data = stbi_loadf(
			p.generic_string().c_str(),
			&width,
			&height,
			&num_channels,
			3               // desired number of channels
		);

		if (!raw_data)
			throw std::runtime_error("Failed to load file");

		Bitmap result(
			width,
			height,
			num_channels,
			e_BitmapFormat::format_u8,
			raw_data
		);

		stbi_image_free(raw_data);

		return result;
	}

	void Bitmap::save(const std::filesystem::path& p) {
		// stb_image_write supports png, bmp, tga, hdr and .jpg
		// TODO - not 100% sure all of this works with format_f32, should test

		if (p.extension() == ".png") {
			stbi_write_png(
				p.generic_string().c_str(),
				static_cast<int>(m_Width),
                static_cast<int>(m_Height),
                static_cast<int>(m_NumChannels),
				m_Data.data(),
				static_cast<int>(m_Width * m_NumChannels * bytes_per_component(m_Format))
			);
		}
		else if (p.extension() == ".bmp") {
			stbi_write_bmp(
				p.generic_string().c_str(),
                static_cast<int>(m_Width),
                static_cast<int>(m_Height),
                static_cast<int>(m_NumChannels),
				m_Data.data()
			);
		}
		else if (p.extension() == ".tga") {
			stbi_write_tga(
				p.generic_string().c_str(),
                static_cast<int>(m_Width),
                static_cast<int>(m_Height),
                static_cast<int>(m_NumChannels),
				m_Data.data()
			);
		}
		else if (p.extension() == ".hdr") {
			// HDR format expects float data, should test with u8 data as well
			stbi_write_hdr(
				p.generic_string().c_str(),
				static_cast<int>(m_Width),
				static_cast<int>(m_Height),
				static_cast<int>(m_NumChannels),
				reinterpret_cast<const float*>(m_Data.data())
			);
		}
		else if (p.extension() == ".jpg") {
			stbi_write_jpg(
				p.generic_string().c_str(),
				static_cast<int>(m_Width),
				static_cast<int>(m_Height),
				static_cast<int>(m_NumChannels),
				m_Data.data(),
				100 // quality
			);
		}
		else
			throw std::runtime_error("Unsupported file format");
	}

	std::ostream& operator << (std::ostream& os, const Bitmap& bmp) {
		os 
			<< "Image(" 
			<< bmp.m_Width 
			<< 'x' 
			<< bmp.m_Height 
			<< 'x' 
			<< bmp.m_Depth 
			<< " c"
			<< bmp.m_NumChannels
			<< bmp.m_Format 
			<< ')';

		return os;
	}

	std::ostream& operator << (std::ostream& os, e_BitmapType type) {
		switch (type) {
		case e_BitmapType::type_2d:   os << "2d";   break;
		case e_BitmapType::type_cube: os << "cube"; break;
		default:
			os << "[UNKNOWN]";
		}

		return os;
	}

	std::ostream& operator << (std::ostream& os, e_BitmapFormat fmt) {
		switch (fmt) {
		case e_BitmapFormat::format_u8:  os << "u8";  break;
		case e_BitmapFormat::format_f32: os << "f32"; break;
		}

		return os;
	}
}