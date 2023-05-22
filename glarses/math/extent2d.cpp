#include "extent2d.h"

#include <ostream>

namespace glarses {
    bool Extent2D::operator == (const Extent2D& e) const {
        return
            m_Width  == e.m_Width &&
            m_Height == e.m_Height;
    }

    bool Extent2D::operator != (const Extent2D& e) const {
        return !(*this == e);
    }

    Extent2D Extent2D::operator + (const Extent2D& e) const {
        return {
            m_Width  + e.m_Width,
            m_Height + e.m_Height
        };
    }

    Extent2D Extent2D::operator - (const Extent2D& e) const {
        return {
            m_Width  - e.m_Width,
            m_Height - e.m_Height
        };
    }

    Extent2D Extent2D::operator * (float scalar) const {
        return {
            static_cast<uint32_t>(static_cast<float>(m_Width)  * scalar),
            static_cast<uint32_t>(static_cast<float>(m_Height) * scalar)
        };
    }

    Extent2D Extent2D::operator / (float scalar) const {
        return {
            static_cast<uint32_t>(static_cast<float>(m_Width)  / scalar),
            static_cast<uint32_t>(static_cast<float>(m_Height) / scalar)
        };
    }

    std::ostream& operator << (std::ostream& os, const Extent2D& e) {
        os << "Extent2D(" << e.m_Width << " x " << e.m_Height << ')';

        return os;
    }
}