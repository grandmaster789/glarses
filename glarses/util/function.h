#ifndef GLARSES_UTIL_FUNCTION_H
#define GLARSES_UTIL_FUNCTION_H

#include <memory>
#include <utility>

namespace glarses {
    // Based on CppWeekly ep 333:
    //      https://www.youtube.com/watch?v=xJSKk_q25oQ
    //
    // NOTE - this resembles Sean Parents' Concept/Implementation style
    // NOTE - noexcept specifiers are missing (call operator could probably be conditionally noexcept)
    // NOTE - this is owning, so it can have full value semantics; non-owning may be more efficient
    // NOTE - constexpr unique_ptr is c++23, which is not fully supported yet on my platform
    //        this ended up being the reason that this can't be constexpr at the moment
    // NOTE - small object optimization is missing and this still does heap allocation,
    //        so this is probably less performant than std::function
    //
    //        it is a good starting point for allocator-aware closure storage though

    template <typename>
    class Function;

    template <typename R, typename...Args>
    class Function<R(Args...)> {
    public:
        Function() = default;

        template <typename t_Callable> // can I make a concept for this?
        Function(t_Callable&& callable) noexcept;

        Function             (const Function& fn);
        Function& operator = (const Function& fn);
        Function             (Function&& fn) noexcept;
        Function& operator = (Function&& fn) noexcept;

        Function             (std::nullptr_t);
        Function& operator = (std::nullptr_t) noexcept;

        R operator ()(Args&&... args) const;

        explicit operator bool() const noexcept;

    private:
        struct Interface {
            virtual ~Interface() = default;

            virtual Interface* clone()         const = 0;
            virtual R          call(Args&&...) const = 0;
        };

        template <typename t_Callable>
        struct Implementation:
            Interface
        {
            explicit Implementation(t_Callable&& callable);

            Interface* clone()              const override;
            R          call(Args&&... args) const override;

            t_Callable m_StoredFunction;
        };

        std::unique_ptr<Interface> m_Storage;
    };
}

#include "function.inl"

#endif
