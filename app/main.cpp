#include "glarses.h"
#include "os/dynamic_library.h"
#include "util/process.h"
//#include <coroutine>

// -------------------------------------------------------------------------------------
struct SpecialFunctionTracker {
    inline SpecialFunctionTracker(const char* name):
        m_Name(name)
    {
        std::cout << "SpecialFunctionTracker[" << m_Name << "] constructor\n";
    }

    inline ~SpecialFunctionTracker() {
        std::cout << "SpecialFunctionTracker[" << m_Name << "] destructor\n";
    }

    inline SpecialFunctionTracker(const SpecialFunctionTracker& sft):
        m_Name(sft.m_Name)
    {
        std::cout << "SpecialFunctionTracker[" << m_Name << "] copy constructor\n";
    }

    inline SpecialFunctionTracker& operator = (const SpecialFunctionTracker& sft) {
        m_Name = sft.m_Name;
        std::cout << "SpecialFunctionTracker[" << m_Name << "] copy assignment\n";
        return *this;
    }

    inline SpecialFunctionTracker(SpecialFunctionTracker&& sft) noexcept:
            m_Name(sft.m_Name)
    {
        std::cout << "SpecialFunctionTracker[" << m_Name << "] move constructor\n";
    }

    inline SpecialFunctionTracker& operator = (SpecialFunctionTracker&& sft) {
        m_Name = sft.m_Name;
        std::cout << "SpecialFunctionTracker[" << m_Name << "] move assignment\n";
        return *this;
    }

    const char* m_Name;
};
// -------------------------------------------------------------------------------------

int main() {
    // by default the global logger just writes to a file --
    //   add a (colorized) console output as well
    using namespace glarses;

    std::cout << std::unitbuf << std::boolalpha; // enable flushing by default and outputting formatted booleans

    os::install_crash_handler();

    // NOTE the log console sink is not attached to the logging facility yet,
    //      as this is done during Engine initialization...
    g_LogDebug << "Launching";

    /// ------- scheduler test

    try {
        std::cout << "Scheduler test\n";
        //IO context;

        //fn(context);
    }
    catch (std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }

    /// -------

    auto& engine = Engine::instance();
    engine.set_application<Application>();

    engine.start();

    g_LogDebug << "Done";
}
