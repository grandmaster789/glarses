#include "glarses.h"

#include <iostream>

int main() {
    std::cout << "Launching\n";

    using namespace glarses;
    {
        Window win; // most initialization happens in the Window construction
        
        // start looking for those glasses
        // (any interaction with the t5::Manager will start the thread that polls the tiltfive system)
        std::cout << "T5 Application: " << t5::Manager::instance().get_application_id() << '\n';

        win.run();

        // this allows for manual waiting
        t5::Manager::instance().destroy();
    }

    // HelloT5Cube 
    // .Initialize()                              -> Application::Initialize()
    //      glfwInit()
    //      .InitializeApplication()              -> HelloT5Cube::InitializeApplication()
    //          .glfwWindowHint(... openGL 4.6)
    //      glfwCreateWindow()
    //      .BindOpenGL()
    //          
    //      .InitializeContext()                  -> HelloT5Cube::InitializeContext()
    //          .InitializeT5()                   -> HelloT5Cube::InitializeT5()
    //              MakeOwned<T5W::Context>()     -> T5Wrapper::Context
    //              context->Create(...)          -> T5Wrapper::Context::Create(...)
    //              ..
    //              ..
    //              ConnectGlasses(glassesIDList[0])
    //                  
    //          [compile shaders]
    //          leftEyeDisplay.Initialize()
    //          rightEyeDisplay.Initialize()
    //          glasses->getIpd()
    //          leftEyePose.setPosition()
    //          rightEyePose.setPosition()
    //          glEnable(...backface culling)
    //          glEnable(...depth tests)
    // 
    // .Run()

    std::cout << "Done\n";
}