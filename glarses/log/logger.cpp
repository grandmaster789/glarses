#include "logger.h"

namespace glarses::log {
    Logger::Logger() {
        m_Worker = ActiveObject::create();
    }

    Logger::Logger(const std::string& filename):
        Logger()
    {
        add(makeFileSink(filename));
    }

    // this class provides a singleton interface, but can also be used as a regular object
    Logger& Logger::instance() noexcept {
        static Logger global_logger("glarses.log");
        return global_logger;
    }

    LogMessage Logger::operator()(
                  e_LogCategory         category,
            const std::source_location& location
    ) noexcept {
        return {
                this,
                category,
                location
        };
    }

    void Logger::add(LogSink sink) noexcept {
        m_Sinks.push_back(std::move(sink));
    }

    void Logger::removeAll() noexcept {
        m_Sinks.clear();
    }

    size_t Logger::getNumSinks() const noexcept {
        return m_Sinks.size();
    }

    void Logger::flush(LogMessage* message) noexcept {
        m_Worker->send([
            info = std::move(message->m_MetaInfo),
            str  = std::move(message->m_Buffer.str()),
            this
        ] {
            for (auto &sink: m_Sinks)
                sink.write(info, str);
        });
    }
}