#include "process.h"
#include <ostream>

namespace glarses {
    std::ostream& operator << (std::ostream& os, const ProcessOutput& po) {
        os
            << "Exit code: "         << po.m_ExitCode
            << "\nProgram output:\n" << po.m_Output;

        return os;
    }
}