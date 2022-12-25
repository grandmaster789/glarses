#pragma once

#include <array>
#include <expected> // C++23
#include <map>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace glarses::util {
	template <typename T>
	concept c_Container = requires(T x) {
		typename std::remove_cvref_t<T>::value_type;
		x.size();
		x.begin();
		x.end();
	};

	template <typename T>
	concept c_Map = c_Container<T> && 
		requires(T x) {
			typename std::remove_cvref_t<T>::mapped_type; // mapped_type typedef is common between std::map, std::multimap and std::unordered_map
		};

	template <typename T>
	concept c_Set = c_Container<T> && !c_Map<T> &&
		requires(T x) {
			typename std::remove_cvref_t<T>::key_type; // sets have a key_type *without* a mapped_type (std::set, std::multiset, std::unordered_set)
		};

	template <typename T>
	concept c_ContainerAdapter = requires(T x) {
		typename std::remove_cvref_t<T>::value_type;
		x.size();
		x.pop();
	};

	template <typename T>
	concept c_Character =
		std::is_same_v<T, char>          ||
		std::is_same_v<T, unsigned char> ||
		std::is_same_v<T, wchar_t>       ||
		std::is_same_v<T, char8_t>       ||
		std::is_same_v<T, char16_t>      ||
		std::is_same_v<T, char32_t>;

	template <typename T> concept c_Integer = std::is_integral_v<T>;
	template <typename T> concept c_Float   = std::is_floating_point_v<T>;

	template <typename T>
	concept c_String = c_Container<T> &&
		requires(T x) {
			requires c_Character<typename T::value_type>;
			x.length();
			x.data();
		};

	template <typename T>
	concept c_OwningString = c_String<T> &&
		requires(T x) {
			x.resize(std::size_t{}); // presence of a resize method indicates ownership (kind of)
		};

	template <typename T>
	concept c_StringView = c_String<T> && 
		!requires(T x) {		
			x.resize(std::size_t{});
		}; // missing resize method indicates this is a non-owning view-like variant

	template <typename T>
	concept c_ContinuousContainer =
		c_OwningString<T> ||                 // either an owning string *OR*
		(c_Container<T> && requires(T x) {   // a Container
			std::span(x);                    // able to construct a span from the object (which implies that it can be represented as pointer + size)
			x.resize(std::size_t{});         // resize method indicates this is an owning container (non-view)
		});

	template <typename T>
	concept c_Tuple = requires(T x) {
		sizeof(std::tuple_size<std::remove_cvref_t<T>>);
		std::get<0>(x);
	};

	template <typename T>
	concept c_Array =
		std::is_array_v<T> && 
		(std::extent_v<std::remove_cvref_t<T>> != 0); // disregard zero-sized or unspecified size arrays (sometimes used for 'flexible arrays' in C)

	template <typename T>
	concept c_Pair = requires(T x) {
		typename std::remove_cvref_t<T>::first_type;
		typename std::remove_cvref_t<T>::second_type;

		x.first;
		x.second;
	};

	template <typename T>
	concept c_Expected = requires(T x) {
		typename std::remove_cvref_t<T>::value_type;
		typename std::remove_cvref_t<T>::error_type;
		typename std::remove_cvref_t<T>::unexpected_type;

		x.has_value();
		x.error();

		// -- only non-void expected have a .value()
		requires 
			std::is_same_v<typename std::remove_cvref_t<T>::value_type,	void> || // either it's expected<void>
			requires (T xx) {
				xx.value(); // or it has .value()
			};
	};

	// decent amount of overlap between expected and optional; expected is more restrictive
	template <typename T>
	concept c_Optional = !c_Expected<T> &&
		requires(T x) {
			typename std::remove_cvref_t<T>::value_type;
			x.value();
			x.has_value();
			x.operator*();
		};

	template <typename T>
	concept c_UniquePtr = 
		requires(T x) {
			typename std::remove_cvref_t<T>::element_type;
			x.operator*();
		} &&
		!requires(T x, T y) {
			x = y;
		};

	// detectors for common STL types
	template <typename T>    constexpr inline bool is_std_basic_string_v                           = false;
	template <typename...Ts> constexpr inline bool is_std_basic_string_v<std::basic_string<Ts...>> = true;
	template <typename T>    constexpr inline bool is_std_vector_v                                 = false;
	template <typename...Ts> constexpr inline bool is_std_vector_v<std::vector<Ts...>>             = true;
	template <typename T>    constexpr inline bool is_std_variant_v                                = false;
	template <typename...Ts> constexpr inline bool is_std_variant_v<std::variant<Ts...>>           = true;

	// concepts based on detectors
	template <typename T>
	concept c_Variant = is_std_variant_v<T>;
}