#include "active_object.h"

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

    void ActiveObject::run() {
        while (!m_Done) {
            Callback fn;

            m_Work.wait_pop(fn);

            fn();
        }
    }
}