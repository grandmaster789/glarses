#ifndef GLARSES_UTIL_TYPEMAP_INL
#define GLARSES_UTIL_TYPEMAP_INL

#include "typemap.h"
#include "algorithm.h"

namespace glarses {
    template <typename T>
    void TypeMap::insert(T* ptr) {
        LockGuard guard(m_Mutex);

        std::type_index key(typeid(T));

        if (!contains(m_Keys, key)) {
            m_Keys  .push_back(key);
            m_Values.push_back(ptr);
        }
        else
            throw std::runtime_error("Cannot store multiple pointers of the same type");
    }

    template <typename T>
    void TypeMap::remove(T*) {
        LockGuard guard(m_Mutex);

        std::type_index key(typeid(T));

        auto it = find(m_Keys, key);

        if (it != m_Keys.end()) {
            size_t position = std::distance(std::cbegin(m_Keys), it);

            m_Keys.erase(it);
            m_Values.erase(std::begin(m_Values) + position);
        }
        else
            throw std::runtime_error("Type could not be found in this container");
    }

    template <typename T>
    const T* TypeMap::get() const {
        std::type_index key(typeid(T));
        auto it = find(m_Keys, key);

        if (it == std::cend(m_Keys))
            return nullptr;
        else {
            size_t position = std::distance(std::cbegin(m_Keys), it);

            return static_cast<const T*>(*(std::cbegin(m_Values) + position));
        }
    }

    template <typename T>
    T* TypeMap::get() {
        std::type_index key(typeid(T));
        auto it = std::find(
                std::begin(m_Keys),
                std::end(m_Keys),
                key
        );

        if (it == std::end(m_Keys))
            return nullptr;
        else {
            size_t position = std::distance(std::begin(m_Keys), it);

            // convert from void* to T*
            return static_cast<T*>(*(std::begin(m_Values) + position));
        }
    }
}

#endif