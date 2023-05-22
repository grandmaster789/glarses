#include "offset2d.h"
#include <ostream>

namespace glarses {
    bool Offset2D::operator == (const Offset2D& o) const {
        return
            m_X == o.m_X &&
            m_Y == o.m_Y;
    }

    bool Offset2D::operator != (const Offset2D& o) const {
        return !(*this == o);
    }

    Offset2D Offset2D::operator + (const Offset2D& o) const {
        return {
            m_X + o.m_X,
            m_Y + o.m_Y
        };
    }

    Offset2D Offset2D::operator - (const Offset2D& o) const {
        return {
            m_X - o.m_X,
            m_Y - o.m_Y
        };
    }

    Offset2D Offset2D::operator * (float scalar) const {
        return {
            static_cast<uint32_t>(static_cast<float>(m_X) * scalar),
            static_cast<uint32_t>(static_cast<float>(m_Y) * scalar)
        };
    }

    Offset2D Offset2D::operator / (float scalar) const {
        return {
            static_cast<uint32_t>(static_cast<float>(m_X) / scalar),
            static_cast<uint32_t>(static_cast<float>(m_Y) / scalar)
        };
    }

    std::ostream& operator << (std::ostream& os, const Offset2D& o) {
        os << "Offset2D(" << o.m_X << ", " << o.m_Y << ')';
        return os;
    }
}