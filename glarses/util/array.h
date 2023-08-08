#ifndef GLARSES_UTIL_ARRAY_H
#define GLARSES_UTIL_ARRAY_H

#include <cstdint>
#include <type_traits>

namespace glarses::util {
    struct AllocatorBase;
}

namespace glarses {
    // dynamically sized array kind of similar to std::pmr::vector, using custom allocators
    // destructive move, no-copy
    // newly allocated elements are *not* initialized
    // not thread safe
    // does not allow types with a constructor/destructor, no support for polymorphism (simple stuff only!)
    // 32-bits capacity/size (suitable for most, but definitely not all payloads)
    template <typename T>
    class Array {
    public:
        static_assert(std::is_trivially_constructible_v<T>);
        static_assert(std::is_trivially_destructible_v<T>);

        Array() = default;
        ~Array();

        explicit Array(
            util::AllocatorBase* allocator,
            uint32_t             initial_capacity = 0,
            uint32_t             initial_size     = 0
        );

        Array             (const Array&) = delete;
        Array& operator = (const Array&) = delete;
        Array             (Array&& a) noexcept;
        Array& operator = (Array&& a) noexcept;

        // push/pop operations apply to the back of the set
        Array&               push(const T& element);
        Array&               push(T&& element);
        Array&               push(const Array<T>& arr);
        Array&               push(Array<T>&& arr) noexcept;

        template <typename... Ts>
        requires(sizeof...(Ts) > 1)
        Array&               push(const Ts&... elements);

        template <typename... Ts>
        requires(sizeof...(Ts) > 1)
        Array&               push(Ts&&... elements);

                      void     pop()           noexcept;                             // drops the last element
        [[nodiscard]] bool     pop(T& element) noexcept;                             // returns whether the element was actually popped
                      void     pop_n(uint32_t num_elements)                noexcept; // drops the last N elements
        [[nodiscard]] uint32_t pop_n(uint32_t num_elements, Array<T>& out) noexcept; // return how many elements were popped

        [[nodiscard]]       T& operator [] (uint32_t index);
        [[nodiscard]] const T& operator [] (uint32_t index) const;
        [[nodiscard]]       T& at(uint32_t index);
        [[nodiscard]] const T& at(uint32_t index) const;
        [[nodiscard]]       T& front();
        [[nodiscard]] const T& front() const;
        [[nodiscard]]       T& back();
        [[nodiscard]] const T& back() const;

        // iterators to allow (some) interop with stl and range-based for loops
        [[nodiscard]]       T* begin();
        [[nodiscard]] const T* begin() const;
        [[nodiscard]]       T* end();
        [[nodiscard]] const T* end() const;

        bool operator == (const Array& arr) const noexcept;
        bool operator != (const Array& arr) const noexcept;

        void clear();
        void swap_delete  (uint32_t index);        // swap an element to the end and delete it (efficient but unstable)
        void stable_delete(uint32_t index);        // remove an element while keeping the order intact (not very efficient)
        void reserve      (uint32_t new_capacity);
        void resize       (uint32_t new_size);     // will discard or allocate as needed; new elements are *uninitialized*

        [[nodiscard]] uint32_t num_elements() const;
        [[nodiscard]] uint32_t capacity() const;

        [[nodiscard]] uint32_t size_in_bytes() const;
        [[nodiscard]] uint32_t capacity_in_bytes() const;

    private:
        T*                   m_Elements        = nullptr;
        uint32_t             m_NumElements     = 0;
        uint32_t             m_ElementCapacity = 0;
        util::AllocatorBase* m_Allocator       = nullptr;
    };

    // non-owning view into an Array (or any contiguous data with the same type)
    // NOTE this is basically a minimal stl range view
    template <typename T>
    struct ArrayView {
        ArrayView(T* data, uint32_t num_elements);

              T& operator [] (uint32_t index);
        const T& operator [] (uint32_t index) const;

        [[nodiscard]]       T* begin();
        [[nodiscard]] const T* begin() const;
        [[nodiscard]]       T* end();
        [[nodiscard]] const T* end() const;

        T*       m_Data;
        uint32_t m_NumElements;
    };
}

#include "array.inl"

#endif
