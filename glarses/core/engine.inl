#ifndef GLARSES_CORE_ENGINE_INL
#define GLARSES_CORE_ENGINE_INL

#include "engine.h"
#include <utility>

namespace glarses {
    template <c_System T, typename... tArgs>
    T* Engine::add(tArgs... args) {
        std::unique_ptr<T> system;

        if constexpr (sizeof...(tArgs) > 0)
            system = std::make_unique<T>(std::forward<tArgs...>(args...));
        else
            system = std::make_unique<T>();

        T* result = system.get();

        m_SystemMap.insert(result);
        m_Systems.push_back(std::move(system));

        return result;
    }

    template <c_System T>
    void Engine::remove(T* system) {
        if (auto* s = m_SystemMap.get<T>()) {
            std::erase_if(
                    m_Systems,
                    [s](const SystemPtr& ptr) {
                        return ptr.get() == s;
                    }
            );

            m_SystemMap.remove(system);
        }
        else
            throw std::runtime_error("Cannot remove subsystem that is not added to the engine");
    }

    template <c_System T>
    const T* Engine::get() const {
        return m_SystemMap.get<T>();
    }

    template <c_System T>
    T* Engine::get() {
        return m_SystemMap.get<T>();
    }

    template <c_Application T, typename... tArgs>
    void Engine::set_application(tArgs... args) {
        if constexpr (sizeof...(tArgs) > 0)
            m_Application = std::make_unique<T>(std::forward<tArgs...>(args...));
        else
            m_Application = std::make_unique<T>();
    }
}

#endif