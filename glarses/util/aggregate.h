#pragma once

#include <string>
#include <type_traits>
#include <stdexcept>
#include <span>

#include "concepts.h"

namespace glarses::util {
	// C++20 only, similar to magic_get, boost::PFR, struct_pack etc.

	template <typename T, typename... Ts>
	requires (std::is_aggregate_v<std::remove_cvref_t<T>>) // this relies on aggregate initialization
	consteval auto count_members();

	auto visit_members(auto&& obj, auto&& visitor);
}

#include "aggregate.inl"