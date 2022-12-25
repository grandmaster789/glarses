#include "../unittest.h"
#include "util/concepts.h"

#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <memory>

namespace test {
	TEST_CASE("concepts", "util") {
		using namespace glarses::util;

		REQUIRE(c_Container<int>                          == false);
		REQUIRE(c_Container<std::vector<int>>             == true);
		REQUIRE(c_Container<std::string>                  == true);

		REQUIRE(c_Map<std::vector<int>>                   == false);
		REQUIRE(c_Map<std::map<int, int>>                 == true);
		REQUIRE(c_Map<std::multimap<int, int>>            == true);
		REQUIRE(c_Map<std::unordered_map<int, int>>       == true);
		REQUIRE(c_Map<std::set<int>>                      == false);

		REQUIRE(c_Set<std::vector<int>>                   == false);
		REQUIRE(c_Set<std::set<int>>                      == true);
		REQUIRE(c_Set<std::multiset<int>>                 == true);
		REQUIRE(c_Set<std::unordered_set<int>>            == true);
		REQUIRE(c_Set<std::map<int, int>>                 == false);

		REQUIRE(c_String<std::string>                     == true);
		REQUIRE(c_String<std::wstring>                    == true);
		REQUIRE(c_String<std::vector<int>>                == false);

		REQUIRE(c_StringView<std::string>                 == false);
		REQUIRE(c_StringView<std::string_view>            == true);

		REQUIRE(c_ContinuousContainer<std::map<int, int>> == false);
		REQUIRE(c_ContinuousContainer<std::vector<int>>   == true);
		REQUIRE(c_ContinuousContainer<std::string>        == true);
		REQUIRE(c_ContinuousContainer<std::string_view>   == false);

		REQUIRE(c_Tuple<int>                              == false);
		REQUIRE(c_Tuple<std::vector<int>>                 == false);
		REQUIRE(c_Tuple<std::tuple<int, int>>             == true);

		REQUIRE(c_Array<int>                              == false);
		REQUIRE(c_Array<int[5]>                           == true);
		REQUIRE(c_Array<int[]>                            == false);

		REQUIRE(c_Pair<int>                               == false);
		REQUIRE(c_Pair<std::pair<int, int>>               == true);
		REQUIRE(c_Pair<std::map<int, int>>                == false);

		REQUIRE(c_Expected<int>                           == false);
		REQUIRE(c_Expected<std::optional<int>>            == false);
		REQUIRE(c_Expected<std::expected<int, int>>       == true);
		REQUIRE(c_Expected<std::expected<void, int>>      == true);

		REQUIRE(c_Optional<int>                           == false);
		REQUIRE(c_Optional<std::expected<int, int>>       == false);
		REQUIRE(c_Optional<std::optional<int>>            == true);

		REQUIRE(c_UniquePtr<int*>                         == false);
		REQUIRE(c_UniquePtr<std::shared_ptr<int>>         == false);
		REQUIRE(c_UniquePtr<std::unique_ptr<int>>         == true);
	}

	TEST_CASE("detectors", "util") {
		using namespace glarses::util;

		REQUIRE(!is_std_basic_string_v<const char*>);
		REQUIRE( is_std_basic_string_v<std::string>);

		REQUIRE(!is_std_vector_v<int[]>);
		REQUIRE( is_std_vector_v<std::vector<int>>);

		REQUIRE(!is_std_variant_v<int>);
		REQUIRE( is_std_variant_v<std::variant<int, bool, float>>);
	}
}