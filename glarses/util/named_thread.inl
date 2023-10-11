#ifndef GLARSES_UTIL_NAMED_THREAD_INL
#define GLARSES_UTIL_NAMED_THREAD_INL

#include "named_thread.h"

namespace glarses::util {
    template <typename t_Fn, typename... t_Args>
    NamedThread::NamedThread(
            const std::string& name,
            t_Fn&&             function,
            t_Args&&...        args
    ) {
        // forwarding parameter packs has weird syntax
        m_Thread = std::jthread(
            [this, fn = std::forward<t_Fn>(function), name, ...fn_args = std::forward<t_Args>(args)] {
                set_name(name);
                fn(fn_args...);
            }
        );
    }
}

#endif
