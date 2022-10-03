#pragma once

#include <filesystem>
#include <vector>
#include <cstdint>

#include "dependencies.h"

// very basic wrapper; should cover just the most common use case
// (always loads as RGB8 texture, no mipmaps)
// -- not particularily efficient or flexible, but easy to use
// 
class Texture {
public:
	Texture() = default;
	~Texture();

	Texture             (const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;
	Texture             (Texture&& t) noexcept;
	Texture& operator = (Texture&& t) noexcept;

	// uses STB to load the file - supports [jpg, png, bmp] at least
	static Texture load_file(const std::filesystem::path& p);
	static Texture load_raw_data(
		const std::vector<uint8_t>& buffer,
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