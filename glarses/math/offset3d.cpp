#include "offset3d.h"
#include "offset2d.h"
#include <ostream>

namespace glarses {
    bool Offset3D::operator == (const Offset3D& o) const {
        return
                m_X == o.m_X &&
                m_Y == o.m_Y &&
                m_Z == o.m_Z;
    }

    bool Offset3D::operator != (const Offset3D& o) const {
        return !(*this == o);
    }

    Offset2D Offset3D::as_Offset2D() const {
        return {
            m_X,
            m_Y
        };
    }

    Offset3D Offset3D::operator + (const Offset3D& o) const {
        return {
                m_X + o.m_X,
                m_Y + o.m_Y,
                m_Z + o.m_Z
        };
    }

    Offset3D Offset3D::operator - (const Offset3D& o) const {
        return {
                m_X - o.m_X,
                m_Y - o.m_Y,
                m_Z - o.m_Z
        };
    }

    Offset3D Offset3D::operator * (float scalar) const {
        return {
                static_cast<uint32_t>(static_cast<float>(m_X) * scalar),
                static_cast<uint32_t>(static_cast<float>(m_Y) * scalar),
                static_cast<uint32_t>(static_cast<float>(m_Z) * scalar)
        };
    }

    Offset3D Offset3D::operator / (float scalar) const {
        return {
                static_cast<uint32_t>(static_cast<float>(m_X) / scalar),
                static_cast<uint32_t>(static_cast<float>(m_Y) / scalar),
                static_cast<uint32_t>(static_cast<float>(m_Z) / scalar)
        };
    }

    std::ostream& operator << (std::ostream& os, const Offset3D& o) {
        os << "Offset3D(" << o.m_X << ", " << o.m_Y << ", " << o.m_Z << ')';
        return os;
    }
}
