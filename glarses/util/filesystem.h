#pragma once

#ifndef GLARSES_UTIL_FILESYSTEM_H
#define GLARSES_UTIL_FILESYSTEM_H

#include <filesystem>
#include <string>
#include <vector>

namespace glarses::util {
    std::string read_text_file (const std::filesystem::path& p);
    size_t      write_text_file(std::string_view sv, const std::filesystem::path& p); // returns number of bytes written

    std::vector<unsigned char> read_binary_file (const std::filesystem::path& p);
    size_t                     write_binary_file(const std::vector<unsigned char>& buffer, const std::filesystem::path& p); // returns number of bytes written
    size_t                     write_binary_file(const void* data, size_t num_bytes, const std::filesystem::path& p);             // returns number of bytes written

    bool   file_exists(const std::filesystem::path& p);
    size_t file_size  (const std::filesystem::path& p);
    void   delete_file(const std::filesystem::path& p);
}

#endif