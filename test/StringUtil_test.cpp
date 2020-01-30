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
//    REQUIRE(StringUtil::Match(text, regex) == true);
//  }

  SECTION("string comment") {
    REQUIRE(StringUtil::IsComment("#asdasd") == true);
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
  	std::string expected = "[ [1, 2, 3], [4, 5, 6], [7, 8, 9] ]";
  	std::vector<std::string> flat_vec;
  	std::string evaluated;
  	for (const auto& vec : nested_vec) {
			flat_vec.emplace_back(StringUtil::Join(", ", "[", "]", vec.begin(), vec.end()));
  	}
  	evaluated = StringUtil::Join(", ", "[ ", " ]", flat_vec.begin(), flat_vec.end());
  	REQUIRE(expected == evaluated);
  }

  SECTION("string leading trim") {
  	std::string evaluated = "   Hello World!";
  	REQUIRE(StringUtil::TrimLeft(evaluated) == "Hello World!");
  }

	SECTION("string trailing trim") {
		std::string evaluated = "Hello World!   ";
		REQUIRE(StringUtil::TrimRight(evaluated) == "Hello World!");
	}

	SECTION("string leading and trailing trim") {
		std::string evaluated = "   Hello World!    ";
		REQUIRE(StringUtil::Trim(evaluated) == "Hello World!");
  }

  SECTION("string to lower case") {
		std::string evaluated = "Hello World!";
		REQUIRE(StringUtil::ToLower(evaluated) == "hello world!");
  }

	SECTION("string to upper case") {
		std::string evaluated = "Hello World!";
		REQUIRE(StringUtil::ToUpper(evaluated) == "HELLO WORLD!");
	}

	SECTION("string last index of") {
		REQUIRE(StringUtil::Index("Hello gorilla", "ll") == 2);
		REQUIRE(StringUtil::LastIndex("Hello gorilla", "ll") == 10);
		REQUIRE(StringUtil::LastIndex("Hello gorilla", "og") == -1);
  }

  SECTION("string repeat") {
  	REQUIRE(StringUtil::Repeat("co", 2) + "nut" == "coconut");
  }
}