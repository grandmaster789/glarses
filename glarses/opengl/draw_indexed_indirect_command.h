#ifndef GLARSES_OPENGL_DRAW_INDIRECT_COMMAND_H
#define GLARSES_OPENGL_DRAW_INDIRECT_COMMAND_H

#include <cstdint>
#include <iosfwd>

namespace glarses::opengl {
    struct DrawIndexedIndirectCommand {
        friend std::ostream& operator << (std::ostream& os, const DrawIndexedIndirectCommand& cmd);

        uint32_t m_IndexCount;
        uint32_t m_InstanceCount;
        uint32_t m_FirstIndex;
        int32_t  m_VertexOffset;

        uint32_t m_FirstInstance;
    };
}

#endif
