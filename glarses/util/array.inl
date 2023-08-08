#ifndef GLARSES_UTIL_ARRAY_INL
#define GLARSES_UTIL_ARRAY_INL

#include "array.h"
#include "memory.h"
#include <cassert>
#include <tuple>

namespace glarses {
    template <typename T>
    Array<T>::~Array() {
        if (m_ElementCapacity > 0)
            dealloc(m_Elements, m_Allocator);
    }

    template <typename T>
    Array<T>::Array(
            util::AllocatorBase* allocator,
            uint32_t             initial_capacity,
            uint32_t             initial_size
    ):
        m_Allocator      (allocator),
        m_ElementCapacity(initial_capacity)
    {
        assert(allocator);
        reserve(initial_capacity);
        resize(initial_size);
    }

    template <typename T>
    Array<T>::Array(Array&& a) noexcept:
        m_Elements       (a.m_Elements),
        m_NumElements    (a.m_NumElements),
        m_ElementCapacity(a.m_ElementCapacity),
        m_Allocator      (a.m_Allocator)
    {
        a.m_Elements      = nullptr;
        a.m_BytesOccupied = 0;
        a.m_BytesCapacity = 0;
        a.m_Allocator     = nullptr;
    }

    template <typename T>
    Array<T>& Array<T>::operator = (Array&& a) noexcept {
        if (m_ElementCapacity > 0)
            dealloc(m_Elements, m_Allocator);

        m_Elements        = a.m_Elements;
        m_NumElements     = a.m_NumElements;
        m_ElementCapacity = a.m_ElementCapacity;
        m_Allocator       = a.m_Allocator;

        a.m_Elements      = nullptr;
        a.m_BytesOccupied = 0;
        a.m_BytesCapacity = 0;
        a.m_Allocator     = nullptr;

        return *this;
    }

    template <typename T>
    Array<T>& Array<T>::push(const T& element) {
        if ((m_NumElements + 1) >= m_ElementCapacity)
            reserve(m_ElementCapacity * 2);

        m_Elements[m_NumElements++] = element;

        return *this;
    }

    template <typename T>
    Array<T>& Array<T>::push(T&& element) {
        if ((m_NumElements + 1) >= m_ElementCapacity) {
            if (m_ElementCapacity == 0)
                reserve(16); // initial guesstimate
            else
                reserve(m_ElementCapacity * 2);
        }

        m_Elements[m_NumElements++] = std::forward<T>(element);

        return *this;
    }

    template <typename T>
    Array<T>& Array<T>::push(const Array<T>& arr) {
        reserve(m_NumElements + arr.num_elements());
        memcpy(
                m_Elements + m_NumElements,
                arr.m_Elements,
                sizeof(T) * arr.m_NumElements
        );

        m_NumElements += arr.num_elements();

        return *this;
    }

    template <typename T>
    Array<T>& Array<T>::push(Array<T>&& arr) noexcept {
        reserve(m_NumElements + arr.num_elements());
        memmove(
                m_Elements + m_NumElements,
                arr.m_Elements,
                sizeof(T) * arr.m_NumElements
        );

        m_NumElements += arr.m_NumElements;
        arr.m_NumElements = 0;

        return *this;
    }

    template <typename T>
    template <typename...Ts>
    requires(sizeof...(Ts) > 1)
    Array<T>& Array<T>::push(const Ts&... elements) {
        static_assert((std::is_constructible_v<T, Ts> && ...));

        if (m_ElementCapacity < m_NumElements + sizeof...(elements))
            reserve(m_ElementCapacity + sizeof...(elements)); // pre-allocate enough memory, but keep the same amount of slack

        ((m_Elements[m_NumElements++] = elements), ...);

        return *this;
    }

    template <typename T>
    template <typename...Ts>
    requires(sizeof...(Ts) > 1)
    Array<T>& Array<T>::push(Ts&&... elements) {
        static_assert((std::is_constructible_v<T, Ts&&> && ...));

        if (m_ElementCapacity < m_NumElements + sizeof...(elements))
            reserve(m_ElementCapacity + sizeof...(elements)); // pre-allocate enough memory, but keep the same amount of slack

        ((m_Elements[m_NumElements++] = std::forward<Ts>(elements)), ...);

        return *this;
    }

    template <typename T>
    void Array<T>::pop() noexcept {
        if (m_NumElements > 0)
            --m_NumElements;
    }

    template <typename T>
    bool Array<T>::pop(T& element) noexcept {
        if (m_NumElements > 0) {
            element = m_Elements[--m_NumElements];
            return true;
        }

        return false;
    }

    template <typename T>
    void Array<T>::pop_n(uint32_t num_elements) noexcept {
        if (num_elements > m_NumElements)
            m_NumElements = 0;
        else
            m_NumElements -= num_elements;
    }

    template <typename T>
    uint32_t Array<T>::pop_n(
            uint32_t  num_elements,
            Array<T>& out
    ) noexcept {
        if (num_elements > m_NumElements) {
            // trying to pop too much -- transfer everything we have instead
            uint32_t result = m_NumElements;
            out.push(std::move(*this));
            return result;
        }
        else {
            uint32_t offset = m_NumElements - num_elements;

            out.reserve(out.num_elements() + num_elements);

            memmove(
                    out.m_Elements + out.m_NumElements,
                    m_Elements + offset,
                    sizeof(T) * num_elements
            );

            out.m_NumElements += num_elements;
            m_NumElements -= num_elements;

            return num_elements;
        }
    }

    template <typename T>
    T& Array<T>::operator [] (uint32_t index) {
        assert(index < m_NumElements);
        return m_Elements[index];
    }

    template <typename T>
    const T& Array<T>::operator [] (uint32_t index) const {
        assert(index < m_NumElements);
        return m_Elements[index];
    }

    template <typename T>
    T& Array<T>::at(uint32_t index) {
        if (index >= m_NumElements)
            throw std::runtime_error("Index out of range");

        return m_Elements[index];
    }

    template <typename T>
    const T& Array<T>::at(uint32_t index) const {
        if (index >= m_NumElements)
            throw std::runtime_error("Index out of range");

        return m_Elements[index];
    }

    template <typename T>
    T& Array<T>::front() {
        assert(m_NumElements > 0);
        return m_Elements[0];
    }

    template <typename T>
    const T& Array<T>::front() const {
        assert(m_NumElements > 0);
        return m_Elements[0];
    }

    template <typename T>
    T& Array<T>::back() {
        assert(m_NumElements > 0);
        return m_Elements[m_NumElements - 1];
    }

    template <typename T>
    const T& Array<T>::back() const {
        assert(m_NumElements > 0);
        return m_Elements[m_NumElements - 1];
    }

    template <typename T>
    T* Array<T>::begin() {
        assert(m_NumElements > 0);
        return m_Elements;
    }

    template <typename T>
    const T* Array<T>::begin() const {
        assert(m_NumElements > 0);
        return m_Elements;
    }

    template <typename T>
    T* Array<T>::end() {
        return m_Elements + m_NumElements;
    }

    template <typename T>
    const T* Array<T>::end() const {
        return m_Elements + m_NumElements;
    }

    template <typename T>
    bool Array<T>::operator == (const Array<T>& arr) const noexcept {
        if (m_NumElements != arr.m_NumElements)
            return false;

        for (uint32_t i = 0; i < m_NumElements; ++i)
            if (m_Elements[i] != arr[i])
                return false;

        return true;
    }

    template <typename T>
    bool Array<T>::operator != (const Array<T>& arr) const noexcept {
        return !(*this == arr);
    }

    template <typename T>
    void Array<T>::clear() {
        m_NumElements = 0;
    }

    template <typename T>
    void Array<T>::swap_delete(uint32_t index) {
        assert(index < m_NumElements);
        m_Elements[index] = m_Elements[--m_NumElements];
    }

    template <typename T>
    void Array<T>::stable_delete(uint32_t index) {
        assert(index < m_NumElements);

        for (size_t i = index; i < m_NumElements - 1; ++i)
            m_Elements[i] = m_Elements[i + 1];

        pop();
    }

    template <typename T>
    void Array<T>::reserve(uint32_t new_capacity) {
        assert(new_capacity >= m_NumElements);

        T* new_elements = alloc<T>(new_capacity, m_Allocator);

        if (m_NumElements > 0) {
            memcpy(new_elements, m_Elements, sizeof(T) * m_NumElements);
            dealloc(m_Elements, m_Allocator);
        }

        m_Elements        = new_elements;
        m_ElementCapacity = new_capacity;
    }

    template <typename T>
    void Array<T>::resize(uint32_t new_size) {
        if (new_size > m_ElementCapacity)
            reserve(new_size);

        m_NumElements = new_size;
    }

    template <typename T>
    uint32_t Array<T>::num_elements() const {
        return m_NumElements;
    }

    template <typename T>
    uint32_t Array<T>::capacity() const {
        return m_ElementCapacity;
    }

    template <typename T>
    uint32_t Array<T>::size_in_bytes() const {
        return m_NumElements * sizeof(T);
    }

    template <typename T>
    uint32_t Array<T>::capacity_in_bytes() const {
        return m_ElementCapacity * sizeof(T);
    }

    // ----------------------------------------------------------------------------
    // ---------------------------------- ArrayView -------------------------------
    // ----------------------------------------------------------------------------

    template <typename T>
    ArrayView<T>::ArrayView(
            T*       data,
            uint32_t num_elements
    ):
        m_Data       (data),
        m_NumElements(num_elements)
    {
    }

    template <typename T>
    T& ArrayView<T>::operator [] (uint32_t index) {
        assert(index < m_NumElements);
        return m_Data[index];
    }

    template <typename T>
    const T& ArrayView<T>::operator [] (uint32_t index) const {
        assert(index < m_NumElements);
        return m_Data[index];
    }

    template <typename T>
    T* ArrayView<T>::begin() {
        return m_Data;
    }

    template <typename T>
    const T* ArrayView<T>::begin() const  {
        return m_Data;
    }

    template <typename T>
    T* ArrayView<T>::end() {
        return m_Data + m_NumElements;
    }

    template <typename T>
    const T* ArrayView<T>::end() const {
        return m_Data + m_NumElements;
    }
}

#endif
