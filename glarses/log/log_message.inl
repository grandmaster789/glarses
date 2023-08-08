#ifndef GLARSES_LOG_LOG_MESSAGE_INL
#define GLARSES_LOG_LOG_MESSAGE_INL

#include "log_message.h"

namespace glarses::log {
    template <typename T>
    LogMessage& LogMessage::operator << (const T& value) {
        m_Buffer << value;
        return *this;
    }
}

#endif
