#ifndef GLARSES_UTIL_ALGORITHM_INL
#define GLARSES_UTIL_ALGORITHM_INL

#include "algorithm.h"
#include <algorithm>
#include <vector>
#include <array>
#include <span>

namespace glarses::util {
    template <typename C, typename V>
        requires (std::convertible_to<V, typename C::value_type>)
    auto find(
        const C& container,
        const V& value
    ) {
        return std::find(
            std::begin(container),
            std::end(container),
            value
        );
    }

    template <typename C, typename P>
    auto find_if(
        const C& container,
        P        predicate
    ) {
        return std::find_if(
            std::begin(container),
            std::end(container),
            predicate
        );
    }

    template <typename C, typename V>
        requires (std::convertible_to<V, typename C::value_type>)
    bool contains(
        const C& container,
        const V& value
    ) {
        return std::find(
            std::begin(container),
            std::end(container),
            value
        ) != std::end(container);
    }

    template <typename C, typename P>
    bool contains_if(
        const C& container,
        P        predicate
    ) {
        return std::find_if(
            std::begin(container),
            std::end(container),
            predicate
        ) != std::end(container);
    }

    template <typename C>
    C set_difference(
        const C& container_a,
        const C& container_b
    ) {
        // create locally sorted copies
        auto a = container_a;
        auto b = container_b;

        std::sort(std::begin(a), std::end(a));
        std::sort(std::begin(b), std::end(b));

        C result;

        std::set_difference(
            std::begin(a), std::end(a),
            std::begin(b), std::end(b),
            std::back_inserter(result)
        );

        return result;
    }

    template <typename T>
    std::vector<T*> weak_copy(const std::vector<std::unique_ptr<T>>& owning_copy) {
        std::vector<T*> result(owning_copy.size());

        for (size_t i = 0; i < owning_copy.size(); ++i)
            result[i] = owning_copy[i].get();

        return result;
    }

    [[nodiscard]] auto stopwatch() {
        struct Timing {
            using Clock     = std::chrono::high_resolution_clock;
            using TimePoint = Clock::time_point;

            Timing() {
                m_StartTime = Clock::now();
            }

            ~Timing() {
                auto delta = Clock::now() - m_StartTime;
                auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(delta);

                std::cout << millis.count() << "ms\n";
            }

            TimePoint m_StartTime;
        };

        return Timing();
    }

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    TriviallyCopyableByteSpan::TriviallyCopyableByteSpan(const T &x):
        std::span<const std::byte>(
            std::as_bytes(
                std::span(&x, static_cast<size_t>(1))
            )
        )
    {
    }

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    TriviallyCopyableByteSpan::TriviallyCopyableByteSpan(std::span<T> x):
        std::span<const std::byte>(
            std::as_bytes(x)
        )
    {
    }

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    TriviallyCopyableByteSpan::TriviallyCopyableByteSpan(std::span<const T> x):
        std::span<const std::byte>(
            std::as_bytes(x)
        )
    {
    }
}

#endif