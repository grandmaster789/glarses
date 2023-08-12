#ifndef GLARSES_UTIL_TYPEMAP_H
#define GLARSES_UTIL_TYPEMAP_H

#include <vector>
#include <typeindex>
#include <typeinfo>
#include <mutex>

namespace glarses {
    // container for weak pointers to unique types, with type-based lookup
    // expecting pretty small map sizes - implemented as flat map with linear probing
    // NOTE - get() is not threadsafe
    class TypeMap {
    public:
        template <typename T>
        void insert(T* ptr); // throws if type is already present

        template <typename T>
        void remove(T* ptr = nullptr); // actual value of the argument is optional but may be useful for type deduction etc

        template <typename T>
        const T* get() const; // will return nullptr if the type is not contained

        template <typename T>
        T* get();

    private:
        using Mutex     = std::mutex;
        using LockGuard = std::lock_guard<Mutex>;

        Mutex                        m_Mutex;
        std::vector<std::type_index> m_Keys;
        std::vector<void*>           m_Values;
    };
}

#include "typemap.inl"

#endif