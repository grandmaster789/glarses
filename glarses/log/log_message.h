#ifndef GLARSES_LOG_LOG_MESSAGE_H
#define GLARSES_LOG_LOG_MESSAGE_H

#include "log_category.h"
#include <string>
#include <sstream>
#include <source_location>

namespace glarses::log {
    /*
     * Aggregate of all available data from a gLog(...) call
     */
    class LogSink;

    // string buffer, accumulates the full message and flushes it during destruction
    //
    class LogMessage {
    private:
        friend class Logger;
        friend class LogSink;

        LogMessage() = default;

        LogMessage(
                Logger*                    owner,
                e_LogCategory              category,
                const std::source_location location = std::source_location::current()
        );

        // no-copy
        LogMessage             (const LogMessage&)     = delete;
        LogMessage& operator = (const LogMessage&)     = delete;
        LogMessage             (LogMessage&&) noexcept;
        LogMessage& operator = (LogMessage&&) noexcept;

    public:
        ~LogMessage();

        // allow additional values to accumulate here
        template <typename T>
        LogMessage& operator << (const T& value);

        // allow iostream manipulator functions to work on this as well (such as std::endl and std::boolalpha)
        LogMessage& operator << (std::ostream& (*fn)(std::ostream&));

        struct MetaInfo {
            e_LogCategory  m_Category;
            std::string    m_SourceFile;
            unsigned int   m_SourceLine;
            unsigned int   m_SourceColumn;
            std::string    m_FunctionName;
        };

    private:
        std::ostringstream m_Buffer;
        Logger*            m_Owner = nullptr;
        MetaInfo           m_MetaInfo;
    };
}

#include "log_message.inl"

#endif
