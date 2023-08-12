#include "glarses.h"

struct Foo:
        glarses::MessageHandler<int>
{
    void operator()(const int& i) override {
        std::cout << "Foo: " << i << '\n';
    }
};

int main() {
    // by default the global logger just writes to a file --
    //   add a (colorized) console output as well
    using namespace glarses;

    os::install_crash_handler();

    g_Log << "Launching";

    {
        Foo f1, f2;

        int i = 666, j = 123;

        broadcast(i);
        broadcast(j);
        broadcast(321);
    }

    auto& engine = Engine::instance();
    engine.set_application<Application>();

    engine.start();

    g_Log << "Done";
}