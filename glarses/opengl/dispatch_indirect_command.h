#ifndef GLARSES_OPENGL_DISPATCH_INDIRECT_COMMAND_H
#define GLARSES_OPENGL_DISPATCH_INDIRECT_COMMAND_H

#include <cstdint>
#include <iosfwd>

namespace glarses::opengl {
    struct DispatchIndirectCommand {
        uint32_t m_GroupCountX;
        uint32_t m_GroupCountY;
        uint32_t m_GroupCountZ;

        friend std::ostream& operator << (std::ostream& os, const DispatchIndirectCommand& dic);
    };
}

#endif