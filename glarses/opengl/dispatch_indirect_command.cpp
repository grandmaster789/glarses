#include "dispatch_indirect_command.h"

#include <ostream>

namespace glarses::opengl {
    std::ostream& operator << (std::ostream& os, const DispatchIndirectCommand& dic) {
        os << "DispatchIndirectCommand("
            << dic.m_GroupCountX << ", "
            << dic.m_GroupCountY << ", "
            << dic.m_GroupCountZ << ")";

        return os;
    }
}
