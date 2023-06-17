#include "draw_indexed_indirect_command.h"
#include <ostream>

namespace glarses::opengl {
    std::ostream& operator << (std::ostream& os, const DrawIndexedIndirectCommand& cmd) {
        os << "DrawIndexedIndirectCommand("
            << cmd.m_IndexCount    << " indices, "
            << cmd.m_InstanceCount << " instances, "
            << cmd.m_FirstIndex    << " i0, "
            << cmd.m_VertexOffset  << " offset, first instance = "
            << cmd.m_FirstInstance << ")";
        
        return os;
    }
}