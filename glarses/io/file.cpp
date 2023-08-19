#include "file.h"
#include "../util/string_util.h"
#include <fstream>

namespace glarses::io {
	std::string read_text_file(
		const std::filesystem::path& p
	) {
		std::string result;

		auto num_bytes = static_cast<size_t>(std::filesystem::file_size(p));
		
		result.resize(num_bytes);

		std::ifstream src(p.string().c_str());
		if (!src.good())
			throw std::runtime_error("Failed to open file input");

		src.read(result.data(), static_cast<std::streamsize>(num_bytes));

		return result;
	}

	void write_text_file(
		const std::filesystem::path& p, 
		const std::string&           text
	) {
		std::ofstream out(p.string().c_str());
		if (!out.good())
			throw std::runtime_error("Failed to open file output");

		out.write(text.data(), static_cast<std::streamsize>(text.size()));
	}

	std::vector<uint8_t> read_binary_file(
		const std::filesystem::path& p
	) {
		std::vector<uint8_t> result;
		size_t num_bytes = std::filesystem::file_size(p);
		result.resize(num_bytes);

		std::ifstream src(p.string().c_str(), std::ios_base::binary);
		if (!src.good())
			throw std::runtime_error("Failed to open file input");

		// stream interface deals in chars, which *should* be bytes but it doesn't hurt to enforce that
		static_assert(sizeof(uint8_t) == sizeof(char)); 
		src.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(num_bytes));

		return result;
	}

	void write_binary_file(
		const std::filesystem::path& p, 
		const std::vector<uint8_t>&  data
	) {
		std::ofstream out(p.string().c_str(), std::ios_base::binary);
		if (!out.good())
			throw std::runtime_error("Failed to open file output");

		static_assert(sizeof(uint8_t) == sizeof(char));
		out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
	}

	void write_binary_file(
		const std::filesystem::path& p, 
		const void*                  data, 
		size_t                       num_bytes
	) {
		std::ofstream out(p.string().c_str(), std::ios_base::binary);
		if (!out.good())
			throw std::runtime_error("Failed to open file output");

		out.write(static_cast<const char*>(data), static_cast<std::streamsize>(num_bytes));
	}
}