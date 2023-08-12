#pragma once

#ifndef GLARSES_UTIL_DETAIL_MEDIATOR_QUEUE_H
#define GLARSES_UTIL_DETAIL_MEDIATOR_QUEUE_H

#include <mutex>
//#include <functional>
#include <vector>
#include "../function.h"

namespace glarses::util::detail {
	// A MessageHandler is either
	//   1. an object with a callable operator that takes the message 
	//   2. a lambda that takes the message
	//   3. a global function that takes the message
	//   
	// NOTE the current implementation doesn't allow for simple detachment of lambdas

	template <typename t_Message>
	class MediatorQueue {
	private:
		MediatorQueue() = default;

	public:
        MediatorQueue             (const MediatorQueue&)    = delete;
        MediatorQueue& operator = (const MediatorQueue&)    = delete;
        MediatorQueue            (MediatorQueue&&) noexcept = delete;
        MediatorQueue& operator= (MediatorQueue&&) noexcept = delete;

		static MediatorQueue<t_Message>& instance() {
			static MediatorQueue<t_Message> mq; // This ends up creating a specific queue for each template instance
			return mq;
		}

		template <typename t_Handler> void attach(t_Handler* h);
		template <typename t_Handler> void detach(t_Handler* h);

		void detach_all();

		void broadcast(const t_Message& message);

	private:
		using Mutex     = std::mutex;
		//using Handler   = std::function<void(const t_Message&)>;
        using Handler = Function<void(const t_Message&)>;

		Mutex                m_Mutex;
		std::vector<Handler> m_Handlers;
		std::vector<void*>   m_SourcePtrs;
	};
}

#include "mediator_queue.inl"

#endif