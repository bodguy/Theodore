#include <Catch2/catch.hpp>

int Factorial( int number ) {
  return number <= 1 ? number : Factorial( number - 1 ) * number;
}

TEST_CASE( "2: Factorial of 0 is 1 (fail)", "[multi-file:2]" ) {
  REQUIRE( Factorial(0) == 1 );
}

TEST_CASE( "2: Factorials of 1 and higher are computed (pass)", "[multi-file:2]" ) {
  REQUIRE( Factorial(1) == 1 );
  REQUIRE( Factorial(2) == 2 );
  REQUIRE( Factorial(3) == 6 );
  REQUIRE( Factorial(10) == 3628800 );
}