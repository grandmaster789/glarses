#pragma once

#include "string.h"
#include <sstream>

namespace util::detail {
	template <
		typename    t_Head, 
		typename... t_Tail
	>
	std::pair<size_t, size_t> find_first_string(
		std::string_view haystack,
		size_t           offset,
		const t_Head&    needle_head,
		const t_Tail&... needle_tail
	) noexcept {
		size_t first = haystack.find(needle_head, offset); // https://en.cppreference.com/w/cpp/string/basic_string_view/find
		size_t last = first + 1;

		if (first == std::string_view::npos) {
			first = haystack.size();
			last = haystack.size();
		}

		if constexpr (sizeof...(t_Tail) != 0) {
			const auto [x, y] = find_first_string(
				haystack,
				offset,
				needle_tail...
			); // tail recursion whoo

			if (x < first) {
				first = x;
				last = y;
			}
		}

		return { first, last };
	}
}

namespace util {
	template <typename...Ts>
	std::vector<std::string> split(
		std::string_view haystack,
		const Ts&...     needles
	) {
		std::vector<std::string> result;

		size_t cursor = 0;

		while (cursor < haystack.size()) {
			const auto [x, y] = detail::find_first_string(haystack, cursor, needles...);

			std::string token{ haystack.substr(cursor, x - cursor) };
			cursor = y;

			if (!token.empty())
				result.push_back(std::move(token));
		}

		return result;
	}

	template <typename... Ts>
	[[nodiscard]] std::string stringify(const Ts&... args) {
		// NOTE const& by default is ok, but may be a pessimization for small types
		// NOTE stringstream is sub-optimal but pretty convenient

		std::stringstream result;

		(result << ... << args);

		return result.str();
	}

	[[nodiscard]] constexpr bool is_upper(char c) noexcept {
		return 
			(c >= 'A') && 
			(c <= 'Z');
	}

	[[nodiscard]] constexpr bool is_lower(char c) noexcept {
		return
			(c >= 'a') &&
			(c <= 'z');
	}

	[[nodiscard]] constexpr bool is_alpha(char c) noexcept {
		return 
			is_upper(c) || 
			is_lower(c);
	}

	[[nodiscard]] constexpr bool is_digit(char c) noexcept {
		return
			(c >= '0') &&
			(c <= '9');
	}

	[[nodiscard]] constexpr bool is_alphanum(char c) noexcept {
		return
			is_alpha(c) ||
			is_digit(c);
	}

	[[nodiscard]] constexpr bool is_linefeed(char c) noexcept {
		// https://en.cppreference.com/w/cpp/language/escape
		return
			(c == '\n') ||	// new line        (should be the most common by far)
			(c == '\r') ||  // carriage return (fairly common in Windows or old Apple plaintext)
			(c == '\f') ||	// form feed       (exceedingly rare, I've never seen this in the wild)
			(c == '\v');	// vertical tab    (exceedingly rare, I've never seen this in the wild)
	}

	[[nodiscard]] constexpr bool is_whitespace(char c) noexcept {
		return
			(c == ' ') || // space
			(c == '\t') || // tab
			is_linefeed(c);
	}
}