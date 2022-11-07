#pragma once

#include "repeating_action.h"

namespace util {
	template <typename Fn>
	RepeatingAction<Fn>::RepeatingAction(Fn&& f):
		Fn(std::move(f))
	{
	}

	template <typename Fn>
	void RepeatingAction<Fn>::set_rate(ms rate) {
		m_Rate = rate;
	}

	template <typename Fn>
	void RepeatingAction<Fn>::tick() {
		auto current = steady_clock::now();

		if (m_LastInvocation + m_Rate <= current) {
			m_LastInvocation = current;

			Fn::operator()();
		}
	}
}
