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
    REQUIRE( buff.get_size() == i+1 );
    REQUIRE( buff.is_empty() == false );
  }
  REQUIRE( buff.is_empty() == false );
  REQUIRE( buff.is_full()  == true );
  REQUIRE( buff.front()    == 0 );

  for (uint8_t i = 0; i < 5; i++) {
    REQUIRE( buff.is_empty() == false );
    REQUIRE( i == buff.pop() );
    REQUIRE( buff.is_full()  == false );
  }

  for (uint8_t i = 10; i < 15; i++) {
    REQUIRE( buff.is_full()  == false );
    buff.push(i);
    REQUIRE( buff.is_empty() == false );
  }

  for (uint8_t i = 0; i < 10; i++) {
    REQUIRE( buff.is_empty() == false );
    REQUIRE( i+5 == buff.pop() );
    REQUIRE( buff.is_full()  == false );
  }

  REQUIRE( buff.is_empty() == true );
  REQUIRE( buff.is_full()  == false );
  REQUIRE( buff.front()    == RingFIFO::EMPTY );
}

TEST_CASE( "Odd inputs are handled", "[RingFIFO]")
{
  {
    RingFIFO buff(0);
    REQUIRE( buff.is_empty() == true );
    REQUIRE( buff.is_full() == true );
  }

  {
    RingFIFO buff(1);
    REQUIRE( buff.is_empty() == true );
    REQUIRE( buff.is_full() == false );
    buff.push(10);
    buff.push(2);
    buff.push(5);
    REQUIRE( buff.get_size() == 1 );
  }

  {
    RingFIFO buff(UINT8_MAX);
    REQUIRE( buff.is_empty() == true );
    REQUIRE( buff.is_full() == false );
    for (uint8_t i = 0; i < UINT8_MAX; i++) {
      buff.push(i);
    }
    REQUIRE( buff.is_full() == true );
    REQUIRE( buff.get_size() == UINT8_MAX-1);
  }
}
