#pragma once

#include <ranges>

namespace glarses::util {
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

    template <typename C>
    C set_difference(
        const C& container_a,
        const C& container_b
    );

    template <typename T>
    std::vector<T*> weak_copy(const std::vector<std::unique_ptr<T>>& owning_copy);
}

#include "algorithm.inl"
