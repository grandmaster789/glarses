#pragma once

namespace glarses::util {
    template <typename T, typename H>
    void attach_handler(H* handler);

    template <typename T, typename H>
    void detach_handler(H* handler);

    template <typename T>
    void detach_all_handlers();

    template <typename T>
    void broadcast(const T& message);

    // [NOTE] this *can* be used as a base class, but it's not required
    template <typename T>
    class MessageHandler {
    public:
        MessageHandler();
        virtual ~MessageHandler();

        MessageHandler             (const MessageHandler&) = default;
        MessageHandler& operator = (const MessageHandler&) = default;
        MessageHandler             (MessageHandler&&) noexcept = default;
        MessageHandler& operator = (MessageHandler&&) noexcept = default;

        virtual void operator()(const T& message) = 0;
    };
}

#include "mediator.inl"