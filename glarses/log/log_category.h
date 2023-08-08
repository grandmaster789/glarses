#ifndef GLARSES_LOG_LOG_CATEGORY_H
#define GLARSES_LOG_LOG_CATEGORY_H

#include <iosfwd>

namespace glarses::log {
    enum class e_LogCategory {
        debug,
        info,
        err,
        warning,
        fatal
    };

    std::ostream& operator << (std::ostream& os, e_LogCategory cat);
}

#endif
