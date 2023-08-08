#include "../unittest.h"
#include "util/string.h"

namespace test {
	TEST_CASE("split", "util::String") {
		using namespace glarses;

		std::string sample = "abc\r\ndef\nghi\nXjkl\r\nmno";
		sample[13] = '\0'; // replace the 'X' with a null character

		auto normalized = normalize(sample);
		
		REQUIRE(normalized == "abc\ndef\nghi\njkl\nmno");

		auto spliced = split(normalized, "\n");

		REQUIRE(spliced.size() == 5);
	}

}