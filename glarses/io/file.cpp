#include "file.h"
#include "../util/string.h"
#include <fstream>

namespace io {
	std::string read_text_file(const std::filesystem::path& p) {
		std::string result;

		size_t num_bytes = static_cast<size_t>(std::filesystem::file_size(p));
		
		result.resize(num_bytes);

		std::ifstream src(p.string().c_str());
		if (!src.good())
			throw std::runtime_error("Failed to open file input");

		src.read(result.data(), num_bytes);

		return result;
	}

	void write_text_file(const std::filesystem::path& p, const std::string& text) {
		std::ofstream out(p.string().c_str());
		if (!out.good())
			throw std::runtime_error("Failed to open file output");

		out.write(text.data(), text.size());
	}

	std::vector<uint8_t> read_binary_file(const std::filesystem::path& p) {
		std::vector<uint8_t> result;
		size_t num_bytes = std::filesystem::file_size(p);
		result.resize(num_bytes);

		std::ifstream src(p.string().c_str(), std::ios_base::binary);
		if (!src.good())
			throw std::runtime_error("Failed to open file input");

		// stream interface deals in chars, which *should* be bytes but it doesn't hurt to enforce thats
		static_assert(sizeof(uint8_t) == sizeof(char)); 
		src.read(reinterpret_cast<char*>(result.data()), num_bytes);

		return result;
	}

	void write_binary_file(const std::filesystem::path& p, const std::vector<uint8_t>& data) {
		std::ofstream out(p.string().c_str(), std::ios_base::binary);
		if (!out.good())
			throw std::runtime_error("Failed to open file output");

		static_assert(sizeof(uint8_t) == sizeof(char));
		out.write(reinterpret_cast<const char*>(data.data()), data.size());
	}
}