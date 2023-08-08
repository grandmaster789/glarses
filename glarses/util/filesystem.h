#ifndef GLARSES_UTIL_FILESYSTEM_H
#define GLARSES_UTIL_FILESYSTEM_H

#include <filesystem>
#include <string>
#include <vector>

namespace glarses {
    // Convenience wrappers around STL filesystem; there are multithreading issues with this, but it's decent enough for now
    // https://en.cppreference.com/w/cpp/filesystem
    //
    // may need to be re-architected with custom memory allocators in mind
    // may be replaced with llfio at some point if we need things like asynchronous filesystem i/o etc

    std::string read_text_file (const std::filesystem::path& p);
    size_t      write_text_file(std::string_view sv, const std::filesystem::path& p); // returns number of bytes written

    std::vector<unsigned char> read_binary_file (const std::filesystem::path& p);
    size_t                     write_binary_file(const std::vector<unsigned char>& buffer, const std::filesystem::path& p); // returns number of bytes written
    size_t                     write_binary_file(const void* data, size_t num_bytes, const std::filesystem::path& p);             // returns number of bytes written

    [[nodiscard]] bool   file_exists(const std::filesystem::path& p);
    [[nodiscard]] size_t file_size  (const std::filesystem::path& p);
                  void   delete_file(const std::filesystem::path& p);

    [[nodiscard]] bool                               directory_exists      (const std::filesystem::path& p);
    [[nodiscard]] std::vector<std::filesystem::path> get_files_in_directory(const std::filesystem::path& p);
    [[nodiscard]] std::vector<std::filesystem::path> get_files_in_directory(const std::filesystem::path& p, std::string_view extension);
    [[nodiscard]] std::vector<std::filesystem::path> get_subdirectories    (const std::filesystem::path& p);
                  void                               create_directory      (const std::filesystem::path& p);
                  void                               delete_directory      (const std::filesystem::path& p);
                  void                               set_current_directory (const std::filesystem::path& p);
    [[nodiscard]] std::filesystem::path              get_current_directory ();
    [[nodiscard]] size_t                             get_free_space();
}

#endif