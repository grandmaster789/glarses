#ifndef GLARSES_MATH_OFFSET3D_H
#define GLARSES_MATH_OFFSET3D_H

#include <cstdint>
#include <iosfwd>

namespace glarses {
    struct Offset3D {
        bool operator == (const Offset3D& o) const;
        bool operator != (const Offset3D& o) const;

        struct Offset2D as_Offset2D() const;

        Offset3D operator + (const Offset3D& o) const;
        Offset3D operator - (const Offset3D& o) const;
        Offset3D operator * (float scalar) const;
        Offset3D operator / (float scalar) const;

        friend std::ostream& operator << (std::ostream& os, const Offset3D& o);

        uint32_t m_X = 0;
        uint32_t m_Y = 0;
        uint32_t m_Z = 0;
    };
}

#endif
