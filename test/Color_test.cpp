#include <Catch2/catch.hpp>
#include "Math/Color.h"

using namespace Theodore;

TEST_CASE("Color unit test") {
	SECTION("color convert") {
		Color c1 = Color::FromHex("#32a852");
		Color c2 = Color(0.196078449f, 0.658823549f, 0.321568638f, 1.f);
		bool tf = (c1 == c2);
		REQUIRE(tf);

		Color c3 = Color::FromRGBA(50, 168, 82, 255);
		tf = (c2 == c3);
		REQUIRE(tf);

		Color c4 = Color::FromHex(0xFF32A852);
		tf = (c3 == c4);
		REQUIRE(tf);

		Color c5 = Color::FromCMYK(70, 0, 51, 34);
		tf = (c4 == c5);
		REQUIRE(tf);

		Color c6 = Color::FromHSV(136, 0.7f, 0.66f);
		tf = (c4 == c6);
		REQUIRE(tf);
	}
}