#include <Catch2/catch.hpp>
#include "Helper/StringUtil.h"
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

  SECTION("string regex match") {
    std::string regex = "#include\\s{1,}\"(.[^\\s]*?)\"";
    REQUIRE(StringUtil::Match(text, regex) == true);
  }

  SECTION("string comment") {
    REQUIRE(StringUtil::IsComment("#asdasd") == true);
  }
}