#ifndef GLARSES_UTIL_STRING_H
#define GLARSES_UTIL_STRING_H

#include <string>
#include <string_view>
#include <vector>

namespace glarses {
	// NOTE this does linear traversals for each of the needles
	template <typename...Ts>
	[[nodiscard]] std::vector<std::string> split(std::string_view haystack, const Ts&... needles);
	[[nodiscard]] std::vector<std::string> split_string(std::string_view haystack, std::string_view needle);

	// NOTE these only do ASCII-style upper/lower casing. Full Unicode support is too complex for this project
	[[nodiscard]] std::string to_lower(std::string_view sv) noexcept;
	[[nodiscard]] std::string to_upper(std::string_view sv) noexcept;

	[[nodiscard]] std::string normalize(std::string_view sv) noexcept; // strips all line feed and null characters except for \n

	[[nodiscard]] std::string concat(const std::vector<std::string>&      parts, std::string_view separator = "");
	[[nodiscard]] std::string concat(const std::vector<const char*>&      parts, std::string_view separator = "");
	[[nodiscard]] std::string concat(const std::vector<std::string_view>& parts, std::string_view separator = "");

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

    [[nodiscard]] constexpr std::string_view trim        (std::string_view sv) noexcept;
    [[nodiscard]] constexpr std::string      align_string(
            std::string_view sv,
            size_t           num_characters     = 80,
            std::string_view delimiter_sequence = "\n\t"
    ) noexcept;

	[[nodiscard]] std::string replace_string(
			std::string_view haystack,
			std::string_view needle,
			std::string_view replacement = ""
	);

    [[nodiscard]] std::wstring to_wide_string(const std::string& src); // NOTE -- assuming ASCII here
}

#include "string_util.inl"

#endif