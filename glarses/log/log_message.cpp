#include "log_message.h"
#include "logger.h"

#include <ostream>

namespace glarses::log {
    LogMessage::LogMessage(
            Logger*                    owner,
            e_LogCategory              category,
            const std::source_location location
    ):
        m_Owner(owner),
        m_MetaInfo {
                .m_Category     = category,
                .m_SourceFile   = location.file_name(),
                .m_SourceLine   = location.line(),
                .m_SourceColumn = location.column(),
                .m_FunctionName = location.function_name()
        }
    {
    }

    LogMessage::~LogMessage() {
        if (m_Owner)
            m_Owner->flush(this);
    }

    LogMessage::LogMessage(LogMessage&& lm) noexcept:
        m_Buffer  (std::move(lm.m_Buffer)),
        m_Owner   (lm.m_Owner),
        m_MetaInfo(lm.m_MetaInfo)
    {
        lm.m_Owner = nullptr;
    }

    LogMessage& LogMessage::operator = (LogMessage&& lm) noexcept {
        if (m_Owner)
            m_Owner->flush(this);

        m_Buffer   = std::move(lm.m_Buffer);
        m_Owner    = lm.m_Owner;
        m_MetaInfo = std::move(lm.m_MetaInfo);

        lm.m_Owner = nullptr;

        return *this;
    }

    // allow iostream manipulator functions to work on this as well (such as std::endl and std::boolalpha)
    LogMessage& LogMessage::operator << (std::ostream& (*fn)(std::ostream&)) {
        (*fn)(m_Buffer);
        return *this;
    }
}