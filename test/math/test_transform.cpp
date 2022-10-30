#include "../unittest.h"
#include "math/transform.h"

namespace test {
	TEST_CASE("as_matrix", "math::Transform") {
		using math::Transform;

		Transform x;

		x
			.set_scale(2, 2, 2)
			.set_position(1, 2, 3);

		glm::mat4 expected_to_parent(
			2, 0, 0, 0,
			0, 2, 0, 0,
			0, 0, 2, 0,
			2, 4, 6, 1
		);

		glm::mat4 expected_to_local(
			 0.5f,  0,     0,    0,
			 0,     0.5f,  0,    0,
			 0,     0,     0.5f, 0,
			-1,    -2,    -3,    1
		);

		REQUIRE(x.get_matrix_to_parent() == expected_to_parent);
		REQUIRE(x.get_matrix_to_local()  == expected_to_local);
	}
}