#include "extent3d.h"
#include "extent2d.h"

#include <ostream>

namespace glarses {
    bool Extent3D::operator == (const Extent3D& e) const {
        return
            m_Width  == e.m_Width &&
            m_Height == e.m_Height &&
            m_Depth  == e.m_Depth;
    }

    bool Extent3D::operator != (const Extent3D& e) const {
        return !(*this == e);
    }

    Extent2D Extent3D::as_Extent2D() const {
        return {
            m_Width,
            m_Height
        };
    }

    Extent3D Extent3D::operator + (const Extent3D& e) const {
        return {
            m_Width  + e.m_Width,
            m_Height + e.m_Height,
            m_Depth  + e.m_Depth
        };
    }

    Extent3D Extent3D::operator - (const Extent3D& e) const {
        return  {
            m_Width  - e.m_Width,
            m_Height - e.m_Height,
            m_Depth  - e.m_Depth
        };
    }

    Extent3D Extent3D::operator * (float scalar) const {
        return {
            static_cast<uint32_t>(static_cast<float>(m_Width)  * scalar),
            static_cast<uint32_t>(static_cast<float>(m_Height) * scalar),
            static_cast<uint32_t>(static_cast<float>(m_Depth)  * scalar)
        };
    }

    Extent3D Extent3D::operator / (float scalar) const {
        return {
            static_cast<uint32_t>(static_cast<float>(m_Width)  / scalar),
            static_cast<uint32_t>(static_cast<float>(m_Height) / scalar),
            static_cast<uint32_t>(static_cast<float>(m_Depth)  / scalar)
        };
    }

    std::ostream& operator << (std::ostream& os, const Extent3D& e) {
        os << "Extent3D(" << e.m_Width << " x " << e.m_Height << " x " << e.m_Depth << ')';

        return os;
    }
}