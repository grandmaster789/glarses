#ifndef GLARSES_MATH_OFFSET2D_H
#define GLARSES_MATH_OFFSET2D_H

#include <cstdint>
#include <iosfwd>

namespace glarses {
    struct Offset2D {
        bool operator == (const Offset2D& o) const;
        bool operator != (const Offset2D& o) const;

        Offset2D operator + (const Offset2D& o) const;
        Offset2D operator - (const Offset2D& o) const;
        Offset2D operator * (float scalar) const;
        Offset2D operator / (float scalar) const;

        friend std::ostream& operator << (std::ostream& os, const Offset2D& o);

        uint32_t m_X = 0;
        uint32_t m_Y = 0;
    };
}

#endif
