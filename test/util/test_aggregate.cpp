#include "../unittest.h"
#include "util/aggregate.h"

namespace {
	struct Foo {
		int         x;
		float       f;
		std::string s;
	};

	struct Empty {};

	struct Bar {
		std::optional<int>             oi;
		std::optional<Foo>             of;
		std::integral_constant<int, 3> ic;
	};
}

namespace test {
	TEST_CASE("count_members", "util::aggrgate") {
		using namespace glarses::util;

		REQUIRE(count_members<Foo>()   == 3);
		REQUIRE(count_members<Empty>() == 0);
		REQUIRE(count_members<Bar>()   == 3);
	}

	TEST_CASE("serialize", "util::aggregate") {
		using namespace glarses::util;

		
	}

}