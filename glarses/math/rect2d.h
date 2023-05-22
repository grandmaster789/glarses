#ifndef GLARSES_MATH_RECT2D_H
#define GLARSES_MATH_RECT2D_H

#include "offset2d.h"
#include "extent2d.h"

namespace glarses {
    struct Rect2D {
        Offset2D m_Offset;
        Extent2D m_Extent;
    };
}

#endif