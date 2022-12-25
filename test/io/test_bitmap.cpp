#include "../unittest.h"
#include "io/bitmap.h"
#include "io/file.h"
#include "dependencies.h"

namespace test {
	using namespace glarses::io;
	namespace fs = std::filesystem;

	fs::path find_assets_folder() {
		fs::path current = fs::current_path();

		while (!current.empty()) {
			if (fs::is_directory(current / "assets"))
				return current / "assets";

			current = current.parent_path();
		}

		throw std::runtime_error("Failed to locate assets folder");
	}


	TEST_CASE("load_image", "io::Bitmap") {
		auto assets = find_assets_folder();

		auto bmp = Bitmap::load(assets / "textures/test_pattern_2x2.png");

		REQUIRE(bmp.get_width()  == 2);
		REQUIRE(bmp.get_height() == 2);

		bmp.save("re-encoded.png");
	}

	TEST_CASE("create_checkerboard", "io::Bitmap") {
		glm::vec4 black(0, 0, 0, 1);
		glm::vec4 white(1, 1, 1, 1);

		Bitmap bmp(
			10,
			10,
			3,
			e_BitmapFormat::format_u8
		);

		int square_size = 2;

		for (int y = 0; y < 10; ++y) {
			for (int x = 0; x < 10; ++x) {
				glm::vec4 color = black;

				if (((y / square_size) % 2) && !((x / square_size) % 2))
					color = white;

				if (!((y / square_size) % 2) && ((x / square_size) % 2))
					color = white;

				bmp.set_pixel(x, y, color);
			}
		}

		bmp.save("checkerboard.bmp");
	}
}