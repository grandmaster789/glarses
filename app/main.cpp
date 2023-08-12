#include "glarses.h"

int main() {
    // by default the global logger just writes to a file --
    //   add a (colorized) console output as well
    using namespace glarses;

    os::install_crash_handler();

    g_Log << "Launching";

    auto& engine = Engine::instance();
    engine.set_application<Application>();

    engine.start();

    g_Log << "Done";
}