#include "catch.hpp"
#include <stdint.h>
#include "RingFIFO.hpp"

TEST_CASE( "Buffer pushes and pops normally", "[RingFIFO]" )
{
  RingFIFO buff(10);
  REQUIRE( buff.is_empty() == true );
  REQUIRE( buff.is_full()  == false );
  for (uint8_t i = 0; i < 10; i++) {
    REQUIRE( buff.is_full()  == false );
    buff.push(i);
    REQUIRE( buff.is_empty() == false );
  }
  REQUIRE( buff.is_empty() == false );
  REQUIRE( buff.is_full()  == true );

  for (uint8_t i = 0; i < 10; i++) {
    REQUIRE( buff.is_empty() == false );
    REQUIRE( i == buff.pop() );
    REQUIRE( buff.is_full()  == false );
  }

  REQUIRE( buff.is_empty() == true );
  REQUIRE( buff.is_full()  == false );
}

