#include "../unittest.h"
#include "io/file.h"
#include "util/string.h"

namespace test {
	TEST_CASE("write_read_text", "io::File") {
		const std::string           sample        = util::normalize("abc\r\ndef\nghi\njkl\r\nmno");
		const std::filesystem::path test_filename = "testing 123.txt";

		io::write_text_file(test_filename, sample);
		
		auto readback = util::normalize(io::read_text_file(test_filename)); // the normalize gets rid of newline and null variations

		REQUIRE(readback == sample);

		std::filesystem::remove(test_filename);
	}

	TEST_CASE("write_read_empty_text", "io::File") {
		const std::string           sample;
		const std::filesystem::path test_filename = "testing 123.txt";

		io::write_text_file(test_filename, sample);

		auto readback = util::normalize(io::read_text_file(test_filename)); // the normalize gets rid of newline and null variations

		REQUIRE(readback == sample);

		std::filesystem::remove(test_filename);
	}

	TEST_CASE("write_read_binary", "io::File") {
		const std::vector<uint8_t> sample = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
		const std::filesystem::path test_filename = "testing 123.bin";

		io::write_binary_file(test_filename, sample);

		auto readback = io::read_binary_file(test_filename);

		REQUIRE(readback == sample);

		std::filesystem::remove(test_filename);
	}

	TEST_CASE("write_read_empty_binary", "io::File") {
		const std::vector<uint8_t> sample;
		const std::filesystem::path test_filename = "testing 123.bin";

		io::write_binary_file(test_filename, sample);

		auto readback = io::read_binary_file(test_filename);

		REQUIRE(readback == sample);

		std::filesystem::remove(test_filename);
	}
}