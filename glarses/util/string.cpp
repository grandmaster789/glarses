#include "string.h"

namespace glarses::util {
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
		std::string_view                separator
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
		std::string_view                separator
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
		std::string_view                     separator
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
}