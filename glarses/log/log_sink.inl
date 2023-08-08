#ifndef GLARSES_LOG_LOG_SINK_INL
#define GLARSES_LOG_LOG_SINK_INL

#include "log_sink.h"

namespace glarses::log {
    template <c_LogSink T>
    LogSink::LogSink(T&& impl):
            m_Wrapper(new Wrapper<T>(std::forward<T>(impl)))
    {
    }

    template <c_LogSink T>
    LogSink::Wrapper<T>::Wrapper(T&& impl):
            m_Impl(std::forward<T>(impl))
    {
    }

    template <c_LogSink T>
    void LogSink::Wrapper<T>::operator()(
            const LogMessage::MetaInfo& info,
            const std::string&          message
    ) {
        m_Impl(info, message);
    }
}

#endif
