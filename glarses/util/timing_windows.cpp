// everything here is specific to windows

#ifdef WIN32

#include "timing.h"

namespace {
    // https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
    LARGE_INTEGER g_Frequency;

    glarses::Clock::time_point g_EngineStartTime;
}

namespace glarses {
    Clock::Clock():
        System("Clock")
    {
        QueryPerformanceFrequency(&g_Frequency);

        g_EngineStartTime = now();
    }

    Clock::time_point Clock::now() {
        LARGE_INTEGER x;
        QueryPerformanceCounter(&x);

        // the ratio is determined once at runtime, which regrettably means that
        // we cannot use the chrono compiletime ratio

        return time_point(
                duration(
                    static_cast<double>(x.QuadPart) /
                    static_cast<double>(g_Frequency.QuadPart)
                )
            );
    }

    Clock::time_point time_now() {
        return Clock::now();
    }

    Clock::duration delta_time(Clock::time_point a, Clock::time_point b) {
        return (b - a);
    }

    Clock::time_point add_time(Clock::time_point t, Clock::duration delta) {
        return t + delta;
    }

    double micros_since(Clock::time_point t) {
        return delta_time(t, time_now()).count() * 1'000'000.0;
    }

    double millis_since(Clock::time_point t) {
        return delta_time(t, time_now()).count() * 1'000.0;
    }

    double seconds_since(Clock::time_point t) {
        return delta_time(t, time_now()).count();
    }

    double seconds_since_start() {
        return delta_time(g_EngineStartTime, time_now()).count();
    }
}

#endif