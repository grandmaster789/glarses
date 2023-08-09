#ifndef GLARSES_LOG_LOGGER_H
#define GLARSES_LOG_LOGGER_H

#include <string>
#include <vector>
#include <source_location>

#include "log_category.h"
#include "log_sink.h"

namespace glarses::log {
    // NOTE each Logger has its own thread to
    class Logger {
    public:
        explicit Logger() = default;
        Logger(const std::string& filename); // log both to a file and to std::cout

        // this class provides a singleton interface, but can also be used as a regular object
        static Logger& instance() noexcept;

        void   add(LogSink sink) noexcept;
        void   removeAll() noexcept;
        size_t getNumSinks() const noexcept;

        void flush(LogMessage* message) noexcept;

        // create an associated LogMessage with the appropriate message metadata
        LogMessage operator()(
                      e_LogCategory         category,
                const std::source_location& location
        ) noexcept;

    private:
        std::vector<LogSink> m_Sinks;
    };
}

// macros to make it as painless as possible to log something
#define g_LogCategory(category) \
	(::glarses::log::Logger::instance()(::glarses::log::e_LogCategory::category, std::source_location::current()))

#define g_Log (g_LogCategory(info))

#define g_LogDebug   (g_LogCategory(debug))
#define g_LogInfo    (g_LogCategory(info))
#define g_LogError   (g_LogCategory(err))
#define g_LogWarning (g_LogCategory(warning))
#define g_LogFatal   (g_LogCategory(fatal))

#endif
