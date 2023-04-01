#include "glarses.h"

#include <iostream>

int main() {
    std::cout << "Launching\n";

    using namespace glarses;

    try {
        Application app; 
        
        // start looking for those glasses
        // (any interaction with the t5::Manager will start the thread that polls the tilt five system)
        std::cout << "T5 Application: " << t5::Manager::instance().get_application_id() << '\n';

        app.run();

        // this allows for manual waiting
        t5::Manager::instance().destroy();
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