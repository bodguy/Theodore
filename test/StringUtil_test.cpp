#include "Helper/StringUtil.h"
#include <Catch2/catch.hpp>
#include <string>

using namespace Theodore;

std::string text = R"(#version 410 core
#include "common.glsl"
in vec3 pos;
void main() {
  gl_Position = vec4(pos, 1.0);
})";

TEST_CASE("StringUtil unit test") {
  SECTION("string contains") {
    std::string dest = "#include";
    REQUIRE(StringUtil::Contains(text, dest) == true);
  }

//  SECTION("string regex match") {
//    std::string regex = "#include\\s{1,}\"(.[^\\s]*?)\"";
//    REQUIRE(StringUtil::Matches(text, regex) == true);
//  }

  SECTION("string comment") {
    REQUIRE(StringUtil::IsComment("#asdasd") == true);
  }

  SECTION("string split with delimiter") {
  	std::vector<std::string> ref_vec;
  	REQUIRE(StringUtil::Split("The quick brown fox jumps over the lazy dog", ref_vec, " ") == 9);
  	REQUIRE(StringUtil::Join(", ", "[ ", " ]", ref_vec.begin(), ref_vec.end()) == "[ The, quick, brown, fox, jumps, over, the, lazy, dog ]");
  }

  SECTION("string joining with vector") {
  	std::vector<std::string> iterable{ "a", "b", "c", "d", "e" };
  	REQUIRE(StringUtil::Join(", ", iterable.begin(), iterable.end()) == "a, b, c, d, e");
  }

  SECTION("string joining with plain old array") {
  	std::string iterable[5] = { "a", "b", "c", "d", "e" };
		REQUIRE(StringUtil::Join(", ", iterable, iterable + 5) == "a, b, c, d, e");
  }

  SECTION("string joining with prefix and suffix") {
		std::string iterable[5] = { "a", "b", "c", "d", "e" };
		REQUIRE(StringUtil::Join(", ", "[ ", " ]", iterable, iterable + 5) == "[ a, b, c, d, e ]");
  }

  SECTION("string joining with prefix and suffix from complex vector container") {
  	std::vector<std::vector<int>> nested_vec = {
			{ 1, 2, 3}, { 4, 5, 6}, { 7, 8, 9}
  	};
  	std::vector<std::string> flat_vec;
  	for (const auto& vec : nested_vec) {
			flat_vec.emplace_back(StringUtil::Join(", ", "[", "]", vec.begin(), vec.end()));
  	}
  	REQUIRE(StringUtil::Join(", ", "[ ", " ]", flat_vec.begin(), flat_vec.end()) == "[ [1, 2, 3], [4, 5, 6], [7, 8, 9] ]");
  }

  SECTION("string leading trim") {
  	REQUIRE(StringUtil::TrimLeft("   Hello World!") == "Hello World!");
  }

	SECTION("string trailing trim") {
		REQUIRE(StringUtil::TrimRight("Hello World!   ") == "Hello World!");
	}

	SECTION("string leading and trailing trim") {
		REQUIRE(StringUtil::Trim("   Hello World!    ") == "Hello World!");
  }

  SECTION("string to lower case") {
		REQUIRE(StringUtil::ToLower("Hello World!") == "hello world!");
  }

	SECTION("string to upper case") {
		REQUIRE(StringUtil::ToUpper("Hello World!") == "HELLO WORLD!");
	}

	SECTION("string last index of") {
		REQUIRE(StringUtil::Index("Hello gorilla", "ll") == 2);
		REQUIRE(StringUtil::LastIndex("Hello gorilla", "ll") == 10);
		REQUIRE(StringUtil::LastIndex("Hello gorilla", "og") == -1);
  }

  SECTION("string repeat") {
		REQUIRE("ba" + StringUtil::Repeat("na", 2) == "banana");
  	REQUIRE(StringUtil::Repeat("co", 2) + "nut" == "coconut");
  }

  SECTION("string occurance count") {
		REQUIRE(StringUtil::Count("every", "e") == 2);
  	REQUIRE(StringUtil::Count("occurance", "cc") == 1);
		REQUIRE(StringUtil::Count("five", "") == 5);
  }
}