#ifndef GLARSES_UTIL_NAMED_THREAD_H
#define GLARSES_UTIL_NAMED_THREAD_H

#include <thread>
#include <string>

namespace glarses::util {
    class NamedThread {
    public:
        NamedThread() = default;

        template <typename t_Fn, typename... t_Args>
        explicit NamedThread(
                const std::string& name,
                t_Fn&&             function,
                t_Args&&...        args
        );

        NamedThread             (NamedThread&& nt) noexcept;
        NamedThread& operator = (NamedThread&& nt) noexcept;

        void join();

    private:
        void set_name(const std::string& name); // platform-specific implementation

        std::jthread m_Thread;
    };
}

#include "named_thread.inl"

#endif
