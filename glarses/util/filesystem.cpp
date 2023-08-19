#include "filesystem.h"

#include <fstream>

namespace glarses {
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

        output.write(sv.data(), static_cast<std::streamsize>(sv.size()));

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

        output.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));

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

        output.write(static_cast<const char*>(data), static_cast<std::streamsize>(num_bytes));

        return output.tellp();
    }

    bool file_exists(
        const std::filesystem::path& p
    ) {
        return
            std::filesystem::exists(p) &&
            std::filesystem::is_regular_file(p);
    }

    size_t file_size(
        const std::filesystem::path& p
    ) {
        return std::filesystem::file_size(p);
    }

    void delete_file(
        const std::filesystem::path& p
    ) {
        if (file_exists(p))
            std::filesystem::remove(p);
        else
            throw std::runtime_error("File does not exist");
    }

    bool directory_exists(const std::filesystem::path& p) {
        return
            std::filesystem::exists(p) &&
            std::filesystem::is_directory(p);
    }

    std::vector<std::filesystem::path> get_files_in_directory(const std::filesystem::path& p) {
        std::vector<std::filesystem::path> result;

        if (!directory_exists(p))
            throw std::runtime_error("Directory does not exist");

        for (std::filesystem::directory_iterator it(p); it != std::filesystem::directory_iterator(); ++it) {
            const auto& current_path = it->path();

            if (std::filesystem::is_block_file(current_path))
                result.push_back(current_path);
        }

        return result;
    }

    std::vector<std::filesystem::path> get_subdirectories(const std::filesystem::path& p) {
        std::vector<std::filesystem::path> result;

        if (!directory_exists(p))
            throw std::runtime_error("Directory does not exist");

        for (std::filesystem::directory_iterator it(p); it != std::filesystem::directory_iterator(); ++it) {
            const auto& current_path = it->path();

            if (std::filesystem::is_directory(current_path))
                result.push_back(current_path);
        }

        return result;
    }

    void create_directory(const std::filesystem::path& p) {
        if (std::filesystem::exists(p))
            throw std::runtime_error("Path already exists");

        std::filesystem::create_directory(p);
    }

    void delete_directory(const std::filesystem::path& p) {
        if (!directory_exists(p))
            throw std::runtime_error("Directory does not exist");

        auto subdirs = get_subdirectories(p);
        if (!subdirs.empty())
            for (const auto& sub: subdirs)
                delete_directory(sub);

        auto files = get_files_in_directory(p);
        for (const auto& f: files)
            delete_file(f);

        std::filesystem::remove(p);
    }

    void set_current_directory(const std::filesystem::path& p) {
        std::filesystem::current_path() = p;
    }

    std::filesystem::path get_current_directory () {
        return std::filesystem::current_path();
    }

    size_t get_free_space() {
        auto info = std::filesystem::space(
                std::filesystem::current_path()
        );

        return info.free;
    }
}