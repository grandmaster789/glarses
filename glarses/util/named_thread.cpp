#include "named_thread.h"
#include "../dependencies.h"
#include "string_util.h"

namespace glarses::util {
    NamedThread::NamedThread(NamedThread&& nt) noexcept:
        m_Thread(std::move(nt.m_Thread))
    {
    }

    NamedThread& NamedThread::operator = (NamedThread&& nt) noexcept {
        m_Thread = std::move(nt.m_Thread);
        return *this;
    }

    void NamedThread::join() {
        m_Thread.join();
    }


    void NamedThread::set_name(const std::string& name) {
#ifdef WIN32
        SetThreadDescription(
                m_Thread.native_handle(),
                to_wide_string(name).c_str()
        );
#else // assume posix
        pthread_setname_np(m_Thread.native_handle(), name.c_str());
#endif
    }
}