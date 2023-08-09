#ifndef GLARSES_UTIL_ACTIVE_OBJECT_H
#define GLARSES_UTIL_ACTIVE_OBJECT_H

#include <thread>
#include <functional>
#include <future>

#include "shared_queue.h"

namespace glarses {
    /*
     * Wrapper object around a separate thread of execution; should be used sparingly
     * NOTE this is not super efficient (involves locks, allocations, enqueueing etc.), but it is safe
     * NOTE this currently only supports fire-and-forget or simple notification, no values yielded, no cancellation etc.
     *
     * (at some point this should probably be upgraded to a multithreaded coroutine scheduler)
     */
    class ActiveObject {
    public:
        using Callback = std::function<void()>;

        static std::unique_ptr<ActiveObject> create();

        virtual ~ActiveObject();

        ActiveObject             (const ActiveObject&&) = delete;
        ActiveObject& operator = (const ActiveObject&&) = delete;
        ActiveObject             (ActiveObject&&) noexcept = delete;
        ActiveObject& operator = (ActiveObject&&) noexcept = delete;

        void              send    (Callback work); // fire-and-forget
        std::future<void> schedule(Callback work); // a future allows us to block until the task was actually executed

    private:
        ActiveObject() = default;

        void run();

        std::jthread          m_Thread;
        bool                  m_Done = false;
        SharedQueue<Callback> m_Work;
    };
}

#endif
