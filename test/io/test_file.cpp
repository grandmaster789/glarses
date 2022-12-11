#include "../unittest.h"
#include "io/file.h"
#include "util/string.h"

namespace test {
	TEST_CASE("write_read_text", "io::File") {
		using namespace glarses::util;
		using namespace glarses::io;
		using namespace std::filesystem;

		const std::string sample        = normalize("abc\r\ndef\nghi\njkl\r\nmno");
		const path        test_filename = "testing 123.txt";

		write_text_file(test_filename, sample);
		
		auto readback = normalize(read_text_file(test_filename)); // the normalize gets rid of newline and null variations

		REQUIRE(readback == sample);

		remove(test_filename);
	}

	TEST_CASE("write_read_empty_text", "io::File") {
		using namespace glarses::io;
		using namespace glarses::util;
		using namespace std::filesystem;

		const std::string sample;
		const path        test_filename = "testing 123.txt";

		write_text_file(test_filename, sample);

		auto readback = normalize(read_text_file(test_filename)); // the normalize gets rid of newline and null variations

		REQUIRE(readback == sample);

		std::filesystem::remove(test_filename);
	}

	TEST_CASE("write_read_binary", "io::File") {
		using namespace glarses::io;
		using namespace std::filesystem;

		const std::vector<uint8_t> sample        = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
		const path                 test_filename = "testing 123.bin";

		write_binary_file(test_filename, sample);

		auto readback = read_binary_file(test_filename);

		REQUIRE(readback == sample);

		remove(test_filename);
	}

	TEST_CASE("write_read_empty_binary", "io::File") {
		using namespace glarses::io;
		using namespace std::filesystem;

		const std::vector<uint8_t> sample;
		const path                 test_filename = "testing 123.bin";

		write_binary_file(test_filename, sample);

		auto readback = read_binary_file(test_filename);

		REQUIRE(readback == sample);

		remove(test_filename);
	}
}