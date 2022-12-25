#pragma once

#include "aggregate.h"

namespace glarses::util {
	namespace detail {
		struct AnyType {
			template <typename T>
			operator T(); // specify that this type can be implicitly converted to any type
		};

		struct AnyInteger {
			template <c_Integer T>
			operator T();
		};

		struct AnyFloat {
			template <c_Float T>
			operator T();
		};

		struct AnyOptional {
			template <c_Optional T>
			operator T();
		};

		struct AnyNullable {
			operator std::nullptr_t();
		};
	}

	template <typename T, typename... Ts>
	requires (std::is_aggregate_v<std::remove_cvref_t<T>>) // this relies on aggregate initialization
	consteval auto count_members() {
		// use concept requirements to try and verify the number of parameters needed to initialize the struct
		// NOTE because all aggregates can be initialized with zero arguments, the test needs to be if it would
		//      work with one additional parameter recursively

		if constexpr (
			requires {
				T{ detail::AnyType{}, Ts{}... };
			} == false
		) {
			return sizeof...(Ts); // would no longer work with any number of additional arguments	
		}
		else
			return count_members<T, detail::AnyType, Ts...>(); // recurse
	}

	auto visit_members(auto&& obj, auto&& visitor) {
		using obj_type = std::remove_cvref_t<decltype(obj)>;

		constexpr auto num_members = count_members<obj_type>();

		// NOTE would be nice if we could do variadic destructuring, but alas we'll have to manually expand
		//      e.g. auto&& [...members] = obj;
		//           return visitor(members...);
		// NOTE there is a proposal in flight for this (P1061), expecting this to get in maybe C++26
		//      https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p1061r3.html
		//
		if constexpr (num_members == 0) {
			return visitor();
		}
		else if constexpr (num_members == 1) {
			auto&& [x0] = obj;
			return visitor(x0);
		}
		else if constexpr (num_members == 2) {
			auto&& [x0, x1] = obj;
			return visitor(x0, x1);
		}
		else if constexpr (num_members == 3) {
			auto&& [x0, x1, x2] = obj;
			return visitor(x0, x1, x2);
		}
		else if constexpr (num_members == 4) {
			auto&& [x0, x1, x2, x3] = obj;
			return visitor(x0, x1, x2, x3);
		}
		else if constexpr (num_members == 5) {
			auto&& [x0, x1, x2, x3, x4] = obj;
			return visitor(x0, x1, x2, x3, x4);
		}
		else if constexpr (num_members == 6) {
			auto&& [x0, x1, x2, x3, x4, x5] = obj;
			return visitor(x0, x1, x2, x3, x4, x5);
		}
		else if constexpr (num_members == 7) {
			auto&& [x0, x1, x2, x3, x4, x5, x6] = obj;
			return visitor(x0, x1, x2, x3, x4, x5, x6);
		}
		else if constexpr (num_members == 8) {
			auto&& [x0, x1, x2, x3, x4, x5, x6, x7] = obj;
			return visitor(x0, x1, x2, x3, x4, x5, x6, x7);
		}
		else
			throw std::runtime_error("Too many members"); // add as needed
	};
}