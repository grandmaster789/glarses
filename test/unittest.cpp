#include "unittest.h"
#include "glarses.h"

namespace test {
	TEST_CASE("square", "dummy") {
		int x = square(5);

		REQUIRE(x == 25); 
	}
}