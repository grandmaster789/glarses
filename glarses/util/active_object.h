#ifndef GLARSES_UTIL_ACTIVE_OBJECT_H
#define GLARSES_UTIL_ACTIVE_OBJECT_H

#include <thread>
#include <functional>
#include <future>

#include "shared_queue.h"

namespace glarses {
    /*
     * Wrapper object around a separate thread of execution; should be used sparingly
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

        void send(Callback work);

    private:
        ActiveObject() = default;

        void run();

        std::jthread          m_Thread;
        bool                  m_Done = false;
        SharedQueue<Callback> m_Work;
    };

    std::future<void> spawn_task(
            std::function<void()> fn,
            ActiveObject*         worker
    );
}

#endif
