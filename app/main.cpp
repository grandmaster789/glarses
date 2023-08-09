#include "glarses.h"

#include "util/memory.h"
#include "util/detail/linear_allocator.h"

#include <iostream>

int main() {
    // by default the global logger just writes to a file --
    //   add a (colorized) console output as well
    using namespace glarses;

    os::install_crash_handler();

    {
        using namespace glarses::log;

        Logger::instance().add(makeStdOutSink());
    }

    g_Log << "Launching";

    Application app;

    app.run();

    g_Log << "Done";
}