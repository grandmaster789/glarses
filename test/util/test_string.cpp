#include "../unittest.h"
#include "util/string_util.h"

namespace test {
	TEST_CASE("split", "util::String") {
		using namespace glarses;

		std::string sample = "abc\r\ndef\nghi\nXjkl\r\nmno";
		sample[13] = '\0'; // replace the 'X' with a null character

		auto normalized = normalize(sample);
		
		REQUIRE(normalized == "abc\ndef\nghi\njkl\nmno");

		auto spliced = split(normalized, "\n");

		REQUIRE(spliced.size() == 5);

        std::string sample2 = "aaa bbba baba baaa";
        auto spliced2 = split(sample2, "a b"); // > ["aa", "bb", "ab", "aaa"]

        REQUIRE(spliced2.size() == 4);
	}

    TEST_CASE("align_string", "util::String") {
        using namespace glarses;

        std::string sample = "0123456789";

        auto aligned = align_string(sample, 3, " ");

        REQUIRE(aligned == "012 345 678 9");
    }
}