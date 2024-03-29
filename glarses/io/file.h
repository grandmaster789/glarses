#include <filesystem>
#include <string>
#include <vector>

namespace glarses::io {
	std::string          read_text_file   (const std::filesystem::path& p);
	void                 write_text_file  (const std::filesystem::path& p, const std::string& text);
	std::vector<uint8_t> read_binary_file (const std::filesystem::path& p);
	void                 write_binary_file(const std::filesystem::path& p, const std::vector<uint8_t>& data);
	void                 write_binary_file(const std::filesystem::path& p, const void* ptr, size_t num_bytes);
}