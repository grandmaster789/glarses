#ifndef GLARSES_UTIL_PROCESS_H
#define GLARSES_UTIL_PROCESS_H

#include <filesystem>
#include <string>
#include <optional>
#include <iosfwd>

namespace glarses {
    struct ProcessOutput {
        friend std::ostream& operator << (std::ostream& os, const ProcessOutput& po);

        unsigned long m_ExitCode = 0;
        std::string   m_Output;
    };

    std::optional<ProcessOutput> execute_process(
            const std::filesystem::path& working_directory,
            const std::filesystem::path& full_executable_path,
            const char*                  arguments = nullptr
    );
}

#endif
