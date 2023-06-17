#ifndef GLARSES_OPENGL_BUFFER_H
#define GLARSES_OPENGL_BUFFER_H

#include <iosfwd>
#include "gltypes.h"

namespace glarses::opengl {
    static constexpr inline uint64_t k_WholeBuffer = ~0ull;

    struct BufferClearInfo {
        friend std::ostream& operator << (std::ostream& os, const BufferClearInfo& info);

        uint64_t       m_Offset       = 0;
        uint64_t       m_Size         = k_WholeBuffer;
        e_Format       m_Format;
        e_UploadFormat m_UploadFormat = e_UploadFormat::AUTO;
        e_UploadType   m_UploadType   = e_UploadType::AUTO;
    };
}

#endif
