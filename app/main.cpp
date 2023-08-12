#include "glarses.h"

int main() {
    // by default the global logger just writes to a file --
    //   add a (colorized) console output as well
    using namespace glarses;

    os::install_crash_handler();

    // NOTE the log console sink is not attached to the logging facility yet,
    //      as this is done during Engine initialization...
    g_LogDebug << "Launching";

    auto& engine = Engine::instance();
    engine.set_application<Application>();

    engine.start();

    g_LogDebug << "Done";
}