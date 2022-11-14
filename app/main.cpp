#include "glarses.h"

#include <iostream>

int main() {
    std::cout << "Launching\n";

    using namespace glarses;
    {
        Application app; 
        
        // start looking for those glasses
        // (any interaction with the t5::Manager will start the thread that polls the tiltfive system)
        std::cout << "T5 Application: " << t5::Manager::instance().get_application_id() << '\n';

        app.run();

        // this allows for manual waiting
        t5::Manager::instance().destroy();
    }

    std::cout << "Done\n";
}