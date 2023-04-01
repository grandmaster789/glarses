#include "filesystem.h"

#include <fstream>

namespace glarses::util {
    namespace fs = std::filesystem;

    std::string read_text_file(
        const fs::path& p
    ) {
        std::ifstream input(p.generic_string().c_str());

        if (!input.good())
            throw std::runtime_error("Failed to open input file");

        input.seekg(0, std::ios_base::end);
        auto num_bytes = input.tellg();

        input.seekg(0, std::ios_base::beg);

        std::string result;
        result.resize(num_bytes);

        input.read(result.data(), num_bytes);

        return result;
    }

    size_t write_text_file(
        std::string_view             sv,
        const std::filesystem::path& p
    ) {
        std::ofstream output(p.generic_string().c_str());

        if (!output.good())
            throw std::runtime_error("Failed to open output file");

        output.write(sv.data(), sv.size());

        return output.tellp();
    }

    std::vector<unsigned char> read_binary_file(
        const std::filesystem::path& p
    ) {
        std::ifstream input(p.generic_string().c_str(), std::ios_base::binary);

        if (!input.good())
            throw std::runtime_error("Failed to open input file");

        input.seekg(0, std::ios_base::end);
        auto num_bytes = input.tellg();

        input.seekg(0, std::ios_base::beg);

        std::vector<unsigned char> result;
        result.resize(num_bytes);

        input.get(reinterpret_cast<char*>(result.data()), num_bytes);

        return result;
    }

    size_t write_binary_file(
        const std::vector<unsigned char>& buffer,
        const std::filesystem::path&      p
    ) {
        std::ofstream output(p.generic_string().c_str(), std::ios_base::binary);

        if (!output.good())
            throw std::runtime_error("Failed to open output file");

        output.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

        return output.tellp();
    }

    size_t write_binary_file(
        const void*                  data,
        size_t                       num_bytes,
        const std::filesystem::path& p
    ) {
        std::ofstream output(p.generic_string().c_str());

        if (!output.good())
            throw std::runtime_error("Failed to open output file");

        output.write(static_cast<const char*>(data), num_bytes);

        return output.tellp();
    }

    bool file_exists(
        const std::filesystem::path& p
    ) {
        return std::filesystem::exists(p);
    }

    // unit test facility functions (should probably not be used in production)
    size_t file_size(
        const std::filesystem::path& p
    ) {
        return std::filesystem::file_size(p);
    }

    void delete_file(
        const std::filesystem::path& p
    ) {
        std::filesystem::remove(p);
    }
}