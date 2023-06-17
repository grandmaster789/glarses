#ifndef GLARSES_OPENGL_DRAW_INDIRECT_COMMAND_H
#define GLARSES_OPENGL_DRAW_INDIRECT_COMMAND_H

#include <cstdint>
#include <iosfwd>

namespace glarses::opengl {
    struct DrawIndirectCommand {
        friend std::ostream& operator << (std::ostream& os, const DrawIndirectCommand& cmd);

        uint32_t m_VertexCount;
        uint32_t m_InstanceCount;
        uint32_t m_FirstVertex;
        uint32_t m_FirstInstance;
    };
}

#endif
