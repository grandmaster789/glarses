#pragma once

#include <string>
#include <string_view>
#include <vector>

// some of the more advanced utility classes from my Scimitar project

namespace util {
	// NOTE this does linear traversals for each of the needles
	template <typename...Ts>
	std::vector<std::string> split(
		std::string_view haystack,
		const Ts&...     needles
	);

	// NOTE these only do ASCII-style upper/lower casing. Full Unicode support is too complex for this project
	std::string to_lower(std::string_view sv) noexcept;
	std::string to_upper(std::string_view sv) noexcept;

	std::string normalize(std::string_view sv) noexcept; // strips all line feed and null characters except for \n

	std::string concat(const std::vector<std::string>&      parts, std::string_view separator = "");
	std::string concat(const std::vector<const char*>&      parts, std::string_view separator = "");
	std::string concat(const std::vector<std::string_view>& parts, std::string_view separator = "");

	// NOTE uses << operator to stringify individual arguments
	template <typename... Ts>
	[[nodiscard]] std::string stringify(const Ts&... args); 

	// NOTE these again assume ASCII-style strings, unicode is a lot more complicated
	[[nodiscard]] constexpr bool is_upper     (char c) noexcept; // A-Z
	[[nodiscard]] constexpr bool is_lower     (char c) noexcept; // a-z
	[[nodiscard]] constexpr bool is_alpha     (char c) noexcept; // A-Z  || a-z
	[[nodiscard]] constexpr bool is_digit     (char c) noexcept; // 0-9
	[[nodiscard]] constexpr bool is_alphanum  (char c) noexcept; // A-Z  || a-z  || 0-9
	[[nodiscard]] constexpr bool is_linefeed  (char c) noexcept; // '\n' || '\r' || '\f' || '\v'
	[[nodiscard]] constexpr bool is_whitespace(char c) noexcept; // ' '  || '\t' || linefeed
}

#include "string.inl"