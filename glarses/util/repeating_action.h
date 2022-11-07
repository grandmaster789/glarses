#pragma once

#include <chrono>

namespace util {
	// create with a void lambda
	template <typename Fn>
	class RepeatingAction:
		public Fn
	{
	public:
		using ms           = std::chrono::milliseconds;
		using steady_clock = std::chrono::steady_clock;
		using time_point   = std::chrono::time_point<steady_clock>;

		RepeatingAction(Fn&& f);

		void set_rate(ms rate);
		void tick();

	private:
		std::chrono::milliseconds m_Rate;
		time_point                m_LastInvocation;
	};
}

#include "repeating_action.inl"