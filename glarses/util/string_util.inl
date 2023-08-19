#ifndef GLARSES_UTIL_STRING_INL
#define GLARSES_UTIL_STRING_INL

#include "string_util.h"
#include <sstream>
#include <algorithm>

namespace glarses::util::detail {
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
		size_t last  = first + std::string_view(needle_head).size();

		if (first == std::string_view::npos) {
			first = haystack.size();
			last  = haystack.size();
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

namespace glarses {
	template <typename...Ts>
	std::vector<std::string> split(
		std::string_view haystack,
		const Ts&...     needles
	) {
		std::vector<std::string> result;

		size_t cursor = 0;

		while (cursor < haystack.size()) {
			const auto [x, y] = util::detail::find_first_string(haystack, cursor, needles...);

			std::string token{ haystack.substr(cursor, x - cursor) };
			cursor = y;

			if (!token.empty())
				result.push_back(std::move(token));
		}

		return result;
	}

	template <typename... Ts>
	std::string stringify(const Ts&... args) {
		// NOTE const& by default is ok, but may be a pessimization for small types
		// NOTE stringstream is suboptimal but pretty convenient

		std::stringstream result;

		(result << ... << args);

		return result.str();
	}

	constexpr bool is_upper(char c) noexcept {
		return 
			(c >= 'A') && 
			(c <= 'Z');
	}

	constexpr bool is_lower(char c) noexcept {
		return
			(c >= 'a') &&
			(c <= 'z');
	}

	constexpr bool is_alpha(char c) noexcept {
		return 
			is_upper(c) || 
			is_lower(c);
	}

	constexpr bool is_digit(char c) noexcept {
		return
			(c >= '0') &&
			(c <= '9');
	}

	constexpr bool is_alphanum(char c) noexcept {
		return
			is_alpha(c) ||
			is_digit(c);
	}

	constexpr bool is_linefeed(char c) noexcept {
		// https://en.cppreference.com/w/cpp/language/escape
		return
			(c == '\n') ||	// new line        (should be the most common by far)
			(c == '\r') ||  // carriage return (fairly common in Windows or old Apple plaintext)
			(c == '\f') ||	// form feed       (exceedingly rare, I've never seen this in the wild)
			(c == '\v');	// vertical tab    (exceedingly rare, I've never seen this in the wild)
	}

	constexpr bool is_whitespace(char c) noexcept {
		return
			(c == ' ') || // space
			(c == '\t') || // tab
			is_linefeed(c);
	}

    constexpr std::string_view trim(std::string_view s) noexcept {
        size_t start = 0;
        size_t end   = s.size() - 1;

        while (is_whitespace(s[start]) && start < end)
            ++start;

        while (is_whitespace(s[end]) && end > start)
            --end;

        return s.substr(start, end - start + 1);
    }

    constexpr std::string align_string(
            std::string_view sv,
            size_t           num_characters,
            std::string_view delimiter_sequence
    ) noexcept {
        std::string result;
        result.reserve(sv.size() + sv.size() * delimiter_sequence.size() / num_characters);

        size_t pass      = 0;
        size_t remaining = sv.size();

        while (remaining > 0) {
            // if we're at the last pass, just add the last bit and we're done
            if (std::min(remaining, num_characters) == remaining) {
                result += sv.substr(pass * num_characters);
                return result;
            }

            // any other pass we need to append the given number of characters together with the delimiter sequence
            result += sv.substr(pass * num_characters, num_characters);
            result += delimiter_sequence;

            remaining -= num_characters;
            ++pass;
        }

		// only reaches here if the input string was empty to begin with
		return result;
    }


}

#endif