#ifndef GLARSES_UTIL_TIME_H
#define GLARSES_UTIL_TIME_H

#include "../core/system.h"
#include <chrono>

namespace glarses {
    // High frequency timing is platform specific
    //
    class Clock:
        public System
    {
    public:
        using Representation = double; // in seconds
        using period         = std::ratio<1>;
        using duration       = std::chrono::duration<Representation, period>;
        using time_point     = std::chrono::time_point<Clock>;

        inline static const bool is_steady = false;

        Clock();

        // NOTE - this will be undefined when the clock isn't initialized yet
        static time_point now();
    };

    // Timing system is just used to correctly initialize;
    // the actual api is free functions; std::chrono types usually do the right thing
    // these are just here to either make things more convenient or more explicit

    Clock::time_point time_now();

    Clock::duration delta_time(Clock::time_point a, Clock::time_point b);

    Clock::time_point add_time(Clock::time_point t, Clock::duration delta);

    // convenience aliases for delta_time with 'now' - add as needed
    double micros_since (Clock::time_point t);
    double millis_since (Clock::time_point t);
    double seconds_since(Clock::time_point t);

    double seconds_since_start();
}

#endif
