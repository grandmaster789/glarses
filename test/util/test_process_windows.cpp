#ifdef WIN32

#include "../unittest.h"
#include "util/process.h"
#include "util/filesystem.h"
#include "util/string.h"
#include <iostream>

namespace test {
    TEST_CASE("subprocess_launch", "util::process") {
        using namespace glarses;

        const std::filesystem::path binary = "unittest_util_test_process_binary.exe";

        auto cwd = get_current_directory();

        if (file_exists(cwd / binary)) {
            // run without arguments
            {
                std::cout << "Running child process(1)\n";

                auto process_result = execute_process(cwd, cwd / binary);
                REQUIRE(process_result.has_value());
                REQUIRE(process_result->m_ExitCode == 123); // test binary should always yield this exit code

                // output can be split by newlines
                auto lines = split(process_result->m_Output, "\n");
                REQUIRE(lines.size() == 3);

                // (on windows we'll also receive return carriages -- trim to get consistent result)
                for (auto& x: lines)
                    x = trim(x);

                // line 0 is the CMake binary name
                // line 1 is the first output of the binary - the number of arguments, which should be 1
                // line 2 is the value of the 1st argument, which should be the full path
                REQUIRE(lines[1] == "1");
            }

            // run with arguments
        }
        else {
            std::cout << "Test process binary not available yet, please compile cmake project 'unittest_util_test_process_binary'\n";
            std::cout << "CWD: " << cwd.generic_string() << '\n';
        }
    }
}

#endif