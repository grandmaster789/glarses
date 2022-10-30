#pragma once

#include <ranges>

namespace util {
    template <typename C, typename V>
        requires (std::convertible_to<V, typename C::value_type>)
    auto find(
        const C& container,
        const V& value
    );

    template <typename C, typename P>
    auto find_if(
        const C& container,
        P        predicate
    );

    template <typename C, typename V>
        requires (std::convertible_to<V, typename C::value_type>)
    bool contains(
        const C& container,
        const V& value
    );

    template <typename C, typename P>
    bool contains_if(
        const C& container,
        P        predicate
    );
}

#include "algorithm.inl"
