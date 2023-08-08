#ifndef GLARSES_CORE_ENGINE_H
#define GLARSES_CORE_ENGINE_H

/*
    The nature of the engine is a 'system' update scheme - a half-sync, half-async scheduler.

    The systems themselves are DLL's, which can be polled and reloaded at any time

    The engine itself manages loading/unloading of the systems, figuring out
    dependencies between systems, centralizing configuration, facilitating communication
    and instrumentation.

    Effectively the engine manages globally accessible parts, to reflect this the functionality
    is put into the top-level glarses namespace.
*/

#include <cstdint>

// TODO move all of the macro stuff to its own header

#define GLARSES_MAKE_VERSION(major, minor, patch) \
    (                                             \
        (((uint32_t)(major)) << 22) |             \
        (((uint32_t)(minor)) << 12) |             \
        (((uint32_t)(patch)))                     \
    )

#define GLARSES_ENGINE_VERSION GLARSES_MAKE_VERSION(0, 1, 0)
#define GLARSES_ENGINE_NAME    "Glarses Engine"

#ifdef WIN32
    // https://learn.microsoft.com/en-us/cpp/build/importing-and-exporting?view=msvc-170#using-__declspec

    #ifdef DLL_EXPORTS
        #define DLL_API __declspec(dllexport)
    #else
        #define DLL_API __declspec(dllimport)
    #endif

    // https://learn.microsoft.com/en-us/cpp/build/exporting-from-a-dll-using-declspec-dllexport
    #define DLL_EXPORT_PREFIX __declspec(dll_export)
#else
#endif

namespace glarses {

}

#endif
