#pragma once

#include "mediator.h"
#include "detail/mediator_queue.h"

namespace glarses::util {
    template <typename T, typename H>
    void attach_handler(H* handler) {
        detail::MediatorQueue<T>::instance().attach(handler);
    }

    template <typename T, typename H>
    void detach_handler(H* handler) {
        detail::MediatorQueue<T>::instance().detach(handler);
    }

    template <typename T>
    void detach_all_handlers() {
        detail::MediatorQueue<T>::instance().detach_all();
    }

    template <typename T>
    void broadcast(const T& message) {
        detail::MediatorQueue<T>::instance().broadcast(message);
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