#ifndef GLARSES_UTIL_SERVICE_H
#define GLARSES_UTIL_SERVICE_H

namespace glarses {
    struct Service {
        virtual ~Service() = default;

        virtual void init(void* /*configuration*/) {}
        virtual void shutdown()                    {}
    };
}

#endif