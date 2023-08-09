#ifndef GLARSES_UTIL_MOVE_ON_COPY_H
#define GLARSES_UTIL_MOVE_ON_COPY_H

namespace glarses {
    /*
     * Somewhat sloppy helper, this converts copying operations to move operations,
     * which helps compatibility with move-only objects
     */
    template <typename T>
    class MoveOnCopy {
    public:
        MoveOnCopy() = default;
        explicit MoveOnCopy(T&& object) noexcept;

        MoveOnCopy             (const MoveOnCopy& moc);
        MoveOnCopy& operator = (const MoveOnCopy& moc);
        MoveOnCopy             (MoveOnCopy&& moc) noexcept;
        MoveOnCopy& operator = (MoveOnCopy&& moc) noexcept;

        void operator()(); // only available when T is callable

              T& get();
        const T& get() const;

        T release();

    private:
        mutable T m_Object;
    };
}

#include "move_on_copy.inl"

#endif
