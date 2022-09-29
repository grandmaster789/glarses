#include "glarses.h"

#include <iostream>

int main() {
    std::cout << "Launching\n";

    using namespace glarses;
    {
        Window win;
        win.run();
    }

    std::cout << "Done\n";
}