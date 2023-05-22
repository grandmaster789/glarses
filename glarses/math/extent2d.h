#ifndef GLARSES_MATH_EXTENT2D_H
#define GLARSES_MATH_EXTENT2D_H

#include <cstdint>
#include <iosfwd>

namespace glarses {
    struct Extent2D {
        bool operator == (const Extent2D& e) const;
        bool operator != (const Extent2D& e) const;

        Extent2D operator + (const Extent2D& e) const;
        Extent2D operator - (const Extent2D& e) const;

        Extent2D operator * (float scalar) const;
        Extent2D operator / (float scalar) const;

        friend std::ostream& operator << (std::ostream& os, const Extent2D& e);

        uint32_t m_Width  = 0;
        uint32_t m_Height = 0;
    };
}

#endif
