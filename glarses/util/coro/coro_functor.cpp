#include "coro_functor.h"

#include <cassert>

namespace glarses::util::coro {
    CoroFunctor::CoroFunctor(CoroFunctor&& cf) noexcept {
        m_Fn     = cf.m_Fn;
        m_Object = cf.m_Object;

        cf.m_Fn     = nullptr;
        cf.m_Object = nullptr;
    }

    CoroFunctor& CoroFunctor::operator = (CoroFunctor&& cf) noexcept {
         m_Fn     = cf.m_Fn;
         m_Object = cf.m_Object;

         cf.m_Fn     = nullptr;
         cf.m_Object = nullptr;

        return *this;
    }

    CoroFunctor::CoroFunctor(void(*free_function)()) noexcept {
        uintptr_t addr = reinterpret_cast<uintptr_t>(free_function);

        assert((addr & k_RegularFunctionPtrBit) == 0);

        m_Fn = reinterpret_cast<void*>(addr | k_RegularFunctionPtrBit);
    }

    void CoroFunctor::operator()() noexcept {
        uintptr_t addr = reinterpret_cast<uintptr_t>(m_Fn);

        if (addr & k_RegularFunctionPtrBit) {
            // must be a regular function - fix the pointer and call the function
            //
            addr &= ~k_RegularFunctionPtrBit; // strip the tag bit

            if (m_Object) {
                auto (*member_function)(void*) = reinterpret_cast<void(*)(void*)>(addr);
                member_function(m_Object);
            }
            else {
                auto (*free_function)() = reinterpret_cast<void(*)()>(addr);
                free_function();
            }
        }
        else {
            // must be a coroutine frame address then - resume it
            auto coroutine = std::coroutine_handle<>::from_address(reinterpret_cast<void *>(addr));

            coroutine.resume();
        }
    }

    bool CoroFunctor::is_coroutine() const noexcept {
        return !(reinterpret_cast<uintptr_t>(m_Fn) & k_RegularFunctionPtrBit);
    }

    std::coroutine_handle<> CoroFunctor::as_coroutine() noexcept {
        return std::coroutine_handle<>::from_address(m_Fn);
    }
}