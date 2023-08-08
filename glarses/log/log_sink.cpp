#include "log_sink.h"
#include "../dependencies.h"

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
            auto now = std::chrono::system_clock::now();

            auto simplified_source_path = std::filesystem::path(info.m_SourceFile)
                    .filename()
                    .string();

            m_File
                    << std::vformat("{0:%T}", std::make_format_args(now))
                    << info.m_Category
                    << message
                    << " (" << simplified_source_path
                    << ": " << info.m_FunctionName
                    << " [" << info.m_SourceLine << ", " << info.m_SourceColumn
                    << "])\n";
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
                << " (" << info.m_FunctionName << ")\n";

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