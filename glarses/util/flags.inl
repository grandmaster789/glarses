#ifndef GLARSES_UTIL_FLAGS_INL_H
#define GLARSES_UTIL_FLAGS_INL_H

#include "flags.h"

namespace glarses::util {
    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>::Flags(T bit) {
        m_Value = static_cast<Storage>(bit);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T> Flags<T>::operator ~ () const {
        return {
            ~m_Value
        };
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator == (const Flags<T>& f) const {
        return m_Value == f.m_Value;
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator == (T bit) const {
        return m_Value == static_cast<Storage>(bit);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator != (const Flags& f) const {
        return !(*this == f);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator != (T bit) const {
        return !(*this == bit);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T> Flags<T>::operator | (const Flags& flags) {
        return {
            m_Value | flags.m_Value
        };
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T> Flags<T>::operator & (const Flags& flags) {
        return {
            m_Value & flags.m_Value
        };
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T> Flags<T>::operator ^ (const Flags& flags) {
        return {
            m_Value ^ flags.m_Value
        };
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator | (T bit) const {
        return m_Value | static_cast<Storage>(bit);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator & (T bit) const {
        return m_Value & static_cast<Storage>(bit);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr bool Flags<T>::operator ^ (T bit) const {
        return m_Value ^ static_cast<Storage>(bit);
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>& Flags<T>::operator |= (const Flags& flags) {
        m_Value |= flags.m_Value;
        return *this;
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>& Flags<T>::operator &= (const Flags& flags) {
        m_Value &= flags.m_Value;
        return *this;
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>& Flags<T>::operator ^= (const Flags& flags) {
        m_Value ^= flags.m_Value;
        return *this;
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>& Flags<T>::operator |= (T bit) {
        m_Value |= static_cast<Storage>(bit);
        return *this;
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>& Flags<T>::operator &= (T bit) {
        m_Value &= static_cast<Storage>(bit);
        return *this;
    }

    template <typename T>
    requires (std::is_enum_v<T>)
    constexpr Flags<T>& Flags<T>::operator ^= (T bit) {
        m_Value ^= static_cast<Storage>(bit);
        return *this;
    }
}

#endif
