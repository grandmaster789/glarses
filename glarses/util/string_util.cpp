#include "string_util.h"

namespace glarses {
	std::string to_lower(std::string_view sv) noexcept {
		std::string result;
		result.reserve(sv.size());

		for (const auto c : sv) {
			if (is_upper(c))
				result += (c - 'A' + 'a');
			else
				result += c;
		}

		return result;
	}

	std::string to_upper(std::string_view sv) noexcept {
		std::string result;
		result.reserve(sv.size());

		for (const auto c : sv) {
			if (is_lower(c))
				result += (c - 'a' + 'A');
			else
				result += c;
		}

		return result;
	}

	std::string normalize(std::string_view sv) noexcept {
		std::string result;
		result.reserve(sv.size()); // assume we're going to copy each character

		for (auto c : sv) {
			if (
				(c == '\r') ||
				(c == '\f') ||
				(c == '\v') ||
				(c == '\0')
			)
				[[unlikely]]
				continue;  
			else
				[[likely]]
				result += c;
		}

		return result;
	}

	std::string concat(
		const std::vector<std::string>& parts,
		std::string_view     separator
	) {
		size_t total = 0;

		for (const auto& part : parts) {
			total += part.size();
			total += separator.size();
		}

		std::string result;
		result.reserve(total);

		for (const auto& part : parts) {
			result.append(part);
			result.append(separator);
		}

		return result;
	}

	std::string concat(
		const std::vector<const char*>& parts,
		std::string_view          separator
	) {
		size_t total = 0;

		for (const auto& part : parts) {
			total += strlen(part); // eww.
			total += separator.size();
		}

		std::string result;
		result.reserve(total);

		for (const auto& part : parts) {
			result.append(part);
			result.append(separator);
		}

		return result;
	}

	std::string concat(
		const std::vector<std::string_view>& parts,
		std::string_view               separator
	) {
		size_t total = 0;

		for (const auto& part : parts) {
			total += part.size();
			total += separator.size();
		}

		std::string result;
		result.reserve(total);

		for (const auto& part : parts) {
			result.append(part);
			result.append(separator);
		}

		return result;
	}

	std::string replace_string(
			std::string_view haystack,
			std::string_view needle,
			std::string_view replacement
	) {
		return concat(split(haystack, needle), replacement);
	}

    std::wstring to_wide_string(const std::string& src) {
        std::wstring result;
        result.resize(src.size());

        // for ASCII this is fine; non-ascii will be converted to '?'
        // (those values would show negative values as characters)
        for (size_t i = 0; i < src.size(); ++i) {
            auto c = src[i];

            if (c >= 0)
                result[i] = static_cast<wchar_t>(c);
            else
                result[i] = L'?';
        }

        return result;
    }
}