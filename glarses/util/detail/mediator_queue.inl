#pragma once

#ifndef GLARSES_UTIL_DETAIL_MEDIATOR_QUEUE_INL
#define GLARSES_UTIL_DETAIL_MEDIATOR_QUEUE_INL

#include "mediator_queue.h"
#include "../algorithm.h"
#include "../../log/logger.h"

namespace glarses::util::detail {
	template <typename T>
	template <typename H>
	void MediatorQueue<T>::attach(H* handler) {
		std::lock_guard guard(m_Mutex);

		m_Handlers.push_back([handler](const T& message) {
			try {
				(*handler)(message);
			}
			catch (std::exception& ex) {
				g_LogError << "Handler exception: " << ex.what();
			}
			catch (...) {
				g_LogError << "Handler exception [unspecified]";
			}
		});

		m_SourcePtrs.push_back(handler);
	}

	template <typename T>
	template <typename H>
	void MediatorQueue<T>::detach(H* handler) {
        std::lock_guard guard(m_Mutex);

		auto it = find(m_SourcePtrs, handler);

		if (it != std::end(m_SourcePtrs)) {
			size_t idx = std::distance(std::cbegin(m_SourcePtrs), it);

			m_SourcePtrs.erase(it);
			m_Handlers.erase(std::begin(m_Handlers) + idx);
		}
		else
			g_LogError << "Tried to remove an unregistered handler";
	}

	template <typename T>
	void MediatorQueue<T>::detach_all() {
        std::lock_guard guard(m_Mutex);

		m_SourcePtrs.clear();
		m_Handlers.clear();
	}

	template <typename T>
	void MediatorQueue<T>::broadcast(const T& message) {
		decltype(m_Handlers) local_copy;

		{
            std::lock_guard guard(m_Mutex);
			local_copy = m_Handlers;
		}

		// by using a local copy of the list we allow  handlers to mutate the set (typically by removing  themselves from the set)
		for (const auto& handler : local_copy)
			handler(message);
	}
}

#endif