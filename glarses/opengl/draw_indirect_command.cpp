#include "draw_indirect_command.h"
#include <ostream>

namespace glarses::opengl {
    std::ostream& operator << (std::ostream& os, const DrawIndirectCommand& cmd) {
        os << "DrawIndirectCommand("
            << cmd.m_VertexCount   << " vertices, "
            << cmd.m_InstanceCount << " instances, "
            << cmd.m_FirstVertex   << " v0, first instance = "
            << cmd.m_FirstInstance << ")";
        
        return os;
    }
}