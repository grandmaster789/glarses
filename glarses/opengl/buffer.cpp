#include "buffer.h"

#include <ostream>

namespace glarses::opengl {
    std::ostream& operator << (std::ostream& os, const BufferClearInfo& info) {
        os << "BufferClearInfo("
            << "\n\toffset        = " << info.m_Offset
            << "\n\tsize          = " << info.m_Size
            << "\n\tformat        = " << info.m_Format
            << "\n\tupload format = " << info.m_UploadFormat
            << "\n\tupload type   = " << info.m_UploadType
            << "\n)";

        return os;
    }
}