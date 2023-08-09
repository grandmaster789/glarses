#ifndef GLARSES_OS_CRASH_HANDLER_H
#define GLARSES_OS_CRASH_HANDLER_H

// interface crash handling; currently windows-only, but is set up so other OSes should also be possible

namespace glarses::os {
    void install_crash_handler();
}

#endif