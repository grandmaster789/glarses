#include "log_sink.h"
#include "../dependencies.h"
#include "../util/string_util.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>

namespace {
    struct FileSink {
        FileSink(const std::filesystem::path& p):
                m_File(p.string())
        {
            if (!m_File.good())
                throw std::runtime_error("Failed to create file sink");
        }

        void operator()(
                const glarses::log::LogMessage::MetaInfo& info,
                const std::string&                        message
        ) noexcept {
            // NOTE std::chrono::high_resolution_clock is underspecified and doesn't actually give extra resolution
            //      it's better to use wall time here, but it does make this unsuitable for nanosecond event registration.
            //      We could make use of the project specific high resolution clock, but for logging purposes this seems
            //      fine.
            auto now = std::chrono::system_clock::now();

            auto simplified_source_path = std::filesystem::path(info.m_SourceFile)
                    .filename()
                    .string();

            std::stringstream first_part;
            std::stringstream last_part;

            // use some filler to (mostly) line up the source information better
            static constexpr size_t k_function_name_filler = 16;
            std::string simplified_source_filler;

            if (simplified_source_path.size() < k_function_name_filler)
                simplified_source_filler = std::string(k_function_name_filler - simplified_source_path.size(), ' ');

            first_part
                    << std::vformat("{0:%H:%M:%OS}", std::make_format_args(now))
                    << info.m_Category
                    << message;

            // remove mostly unhelpful decorators from the function name
            auto simplified_function_name = glarses::replace_string(info.m_FunctionName, "__cdecl ");

            last_part
                    << "(" << simplified_source_path << simplified_source_filler
                    << " @" << simplified_function_name
                    << " [" << info.m_SourceLine << ", " << info.m_SourceColumn
                    << "])";

            // first part should aligned; we're trying to keep 'normal' usage to about 80 character aligned
            // and the last part is around 20 characters, so we're left with 60
            static constexpr size_t k_text_alignment = 60;

            auto aligned   = glarses::align_string(first_part.str(), k_text_alignment);
            auto filler    = std::string(k_text_alignment - aligned.size() % k_text_alignment, ' ');

            if (m_File.good())
                m_File
                << aligned
                << filler
                << last_part.str()
                << std::endl; // this is a logging facility -- flush a lot
        }

        std::ofstream m_File;
    };
}

namespace glarses::log {
    void LogSink::write(
            const LogMessage::MetaInfo& info,
            const std::string&          message
    ) {
        (*m_Wrapper)(info, message);
    }

    LogSink makeStdOutSink() {
        return [](
                const LogMessage::MetaInfo& info,
                const std::string&          message
        ) {
            switch (info.m_Category) {
                case e_LogCategory::debug:   std::cout << rang::fgB::green;                     break;
                case e_LogCategory::warning: std::cout << rang::fgB::yellow;                    break;
                case e_LogCategory::err:     std::cout << rang::fgB::red;                       break;
                case e_LogCategory::fatal:   std::cout << rang::fgB::red << rang::style::blink; break;
                default:
                    // 'info' gets default color+style
                    break;
            }

            std::cout
                << info.m_Category
                << message
                << '\n';
                //<< " (" << info.m_FunctionName << ")\n";

            std::cout << rang::style::reset;
        };
    }

    LogSink makeStdErrSink() {
        return [](
                const LogMessage::MetaInfo& info,
                const std::string& message
        ) {
            std::cerr << info.m_Category << message << "\n";
        };
    }

    LogSink makeFileSink(const std::filesystem::path& p) {
        return FileSink(p);
    }
}