#include "glarses.h"

#include "util/memory.h"
#include "util/detail/linear_allocator.h"

#include <iostream>

int main() {
    // by default the global logger just writes to a file --
    //   add a (colorized) console output as well
    using namespace glarses;

    {
        using namespace glarses::log;

        Logger::instance().add(makeStdOutSink());
    }

    g_Log << "Launching";

    try {
        Application app;

        app.run();
    }
    catch (std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << '\n';
    }
    catch (std::exception& ex) {
        std::cerr << "Exception thrown: " << ex.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown exception thrown\n";
    }

    g_Log << "Done";
}