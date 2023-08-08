#include "glarses.h"

#include "util/memory.h"
#include "util/detail/linear_allocator.h"

#include <iostream>

int main() {
    std::cout << "Launching\n";

    using namespace glarses;

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

    std::cout << "Done\n";
}