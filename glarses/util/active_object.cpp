#include "active_object.h"
#include "move_on_copy.h"

namespace glarses {
    std::unique_ptr<ActiveObject> ActiveObject::create() {
        std::unique_ptr<ActiveObject> result(new ActiveObject);

        result->m_Thread = std::jthread(
                &ActiveObject::run,
                result.get()
        );

        return result;
    }

    ActiveObject::~ActiveObject() {
        send([this] { m_Done = true; });
    }

    void ActiveObject::send(Callback work) {
        m_Work.push(std::forward<Callback>(work));
    }

    std::future<void> ActiveObject::schedule(Callback work) {
        std::packaged_task<void()> task(std::move(work));

        auto result = task.get_future();

        m_Work.push(MoveOnCopy(std::move(task)));

        return result;
    }

    void ActiveObject::run() {
        while (!m_Done) {
            Callback fn;

            m_Work.wait_pop(fn);

            fn();
        }
    }
}