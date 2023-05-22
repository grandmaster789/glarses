#ifndef GLARSES_UTIL_FLAGS_H
#define GLARSES_UTIL_FLAGS_H

#include <concepts>

namespace glarses::util {
    template <typename t_Bits>
    requires (std::is_enum_v<t_Bits>)
    struct Flags {
        using Storage = std::underlying_type_t<t_Bits>;

        constexpr          Flags() = default;
        constexpr explicit Flags(t_Bits bit);

        constexpr bool operator == (const Flags& f)   const;
        constexpr bool operator == (t_Bits       bit) const;
        constexpr bool operator != (const Flags& f)   const;
        constexpr bool operator != (t_Bits       bit) const;

        constexpr Flags operator ~ () const;

        constexpr Flags operator | (const Flags& flags);
        constexpr Flags operator & (const Flags& flags);
        constexpr Flags operator ^ (const Flags& flags);
        constexpr bool operator | (t_Bits bit) const;
        constexpr bool operator & (t_Bits bit) const;
        constexpr bool operator ^ (t_Bits bit) const;

        constexpr Flags& operator |= (const Flags& flags);
        constexpr Flags& operator &= (const Flags& flags);
        constexpr Flags& operator ^= (const Flags& flags);
        constexpr Flags& operator |= (t_Bits bit);
        constexpr Flags& operator &= (t_Bits bit);
        constexpr Flags& operator ^= (t_Bits bit);

        Storage m_Value = {};
    };
}

#include "flags.inl"

#endif
