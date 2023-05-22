#ifndef GLARSES_EXTENT3D_H
#define GLARSES_EXTENT3D_H

#include <cstdint>
#include <iosfwd>

namespace glarses {
    struct Extent3D {
        bool operator == (const Extent3D& e) const;
        bool operator != (const Extent3D& e) const;

        [[nodiscard]] struct Extent2D as_Extent2D() const;

        Extent3D operator + (const Extent3D& e) const;
        Extent3D operator - (const Extent3D& e) const;
        Extent3D operator * (float scalar) const;
        Extent3D operator / (float scalar) const;

        friend std::ostream& operator << (std::ostream& os, const Extent3D& e);

        uint32_t m_Width  = 0;
        uint32_t m_Height = 0;
        uint32_t m_Depth  = 0;
    };
}

#endif
