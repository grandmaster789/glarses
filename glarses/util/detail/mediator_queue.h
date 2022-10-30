#pragma once

#include <vector>
#include <mutex>
#include <functional>

namespace util::detail {
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
		static MediatorQueue& instance();

		template <typename t_Handler> void attach(t_Handler* h);
		template <typename t_Handler> void detach(t_Handler* h);

		void detach_all();

		void broadcast(const t_Message& message);

	private:
		using Mutex = std::mutex;
		using LockGuard = std::lock_guard<Mutex>;
		using Handler = std::function<void(const t_Message&)>;

		Mutex m_Mutex;
		std::vector<Handler> m_Handlers;
		std::vector<void*>   m_SourcePtrs;
	};
}

#include "mediator_queue.inl"