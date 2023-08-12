#ifndef GLARSES_UTIL_FUNCTION_H
#define GLARSES_UTIL_FUNCTION_H

#include <memory>
#include <utility>

namespace glarses {
    template <typename>
    class Function;

    template <typename R, typename...Args>
    class Function<R(Args...)> {
    private:
        struct Interface {
            virtual ~Interface() = default;

            virtual Interface* clone()         const = 0;
            virtual R          call(Args&&...) const = 0;
        };

        template <typename U>
        struct Implementation:
                Interface
        {
            explicit Implementation(U&& callable):
                m_StoredFunction(std::forward<U>(callable))
            {
            }

            Interface* clone() const override {
                return new Implementation(*this);
            }

            R call(Args&&... args) const override {
                return std::invoke(
                        m_StoredFunction,
                        std::forward<Args>(args)...
                );
            }

            U m_StoredFunction;
        };

    public:
        Function() = default;

        template <typename U>
        Function(U&& callable) noexcept:
            m_Storage(new Implementation<U>(std::forward<U>(callable)))
        {
        }

        Function (const Function& fn):
            m_Storage(fn.m_Storage->clone())
        {
        }

        Function& operator = (const Function& fn) {
            m_Storage.reset(fn.m_Storage->clone());
            return *this;
        }

        Function (Function&& fn) noexcept:
            m_Storage(std::move(fn.m_Storage))
        {
        }

        Function& operator = (Function&& fn) noexcept {
            m_Storage = std::move(fn.m_Storage);
            return *this;
        }

        R operator ()(Args&&... args) const {
            return m_Storage->call(std::forward<Args>(args)...);
        }

    private:
        std::unique_ptr<Interface> m_Storage;
    };
}

#endif
