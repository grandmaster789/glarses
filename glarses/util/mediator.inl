#pragma once

#ifndef GLARSES_UTIL_MEDIATOR_INL
#define GLARSES_UTIL_MEDIATOR_INL

#include "mediator.h"
#include "detail/mediator_queue.h"

namespace glarses {
    template <typename T, typename H>
    void attach_handler(H* handler) {
        util::detail::MediatorQueue<T>::instance().attach(handler);
    }

    template <typename T, typename H>
    void detach_handler(H* handler) {
        util::detail::MediatorQueue<T>::instance().detach(handler);
    }

    template <typename T>
    void detach_all_handlers() {
        util::detail::MediatorQueue<T>::instance().detach_all();
    }

    template <typename T>
    void broadcast(const T& message) {
        util::detail::MediatorQueue<T>::instance().broadcast(message);
    }

    template <typename T>
    MessageHandler<T>::MessageHandler() {
        attach_handler<T>(this);
    }

    template <typename T>
    MessageHandler<T>::~MessageHandler() {
        detach_handler<T>(this);
    }
}

#endif