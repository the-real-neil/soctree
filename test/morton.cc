/* octree/test/morton.cc */

#include <iostream>
#include <cstdlib>
#include <cassert>

#include "config.h"

namespace
{
  struct _rgba_type
  {
#if 0
    /*  */
#elif ( BYTE_ORDER == LITTLE_ENDIAN )
    uint8_t r ;
    uint8_t g ;
    uint8_t b ;
    uint8_t a ;
#elif ( BYTE_ORDER == BIG_ENDIAN )
    uint8_t a ;
    uint8_t b ;
    uint8_t g ;
    uint8_t r ;
#else
#  error "unsupported endianness"
#endif
  };

  union _rgba_union
  {
    int32_t int32_ ;
    _rgba_type rgba_ ;
  };

  _rgba_type
  _rgba( int32_t n )
  {
    _rgba_union un ;
    un.int32_ = n ;
    return un.rgba_ ;
  }

  _rgba_type
  _rgba( uint8_t r, uint8_t g, uint8_t b )
  {
    _rgba_type ret ;
    ret.r = r ;
    ret.g = g ;
    ret.b = b ;
    ret.a = 0 ;
    return ret ;
  }

  int32_t
  _int32( _rgba_type const&o )
  {
    _rgba_union un ;
    un.rgba_ = o ;
    return un.int32_ ;
  }
  
  int32_t
  _int32( uint8_t r, uint8_t g, uint8_t b )
  {
    return _int32( _rgba( r, g, b ) );
  }
  
  int32_t
  _morton( uint8_t r, uint8_t g, uint8_t b )
  {
    int32_t ret = 0 ;
    for( size_t i = 0 ; i < 8 ; ++i ){
      ret |= 0
        | ((r & ((uint8_t)1 << i)) << (2*i + 0))
        | ((g & ((uint8_t)1 << i)) << (2*i + 1))
        | ((b & ((uint8_t)1 << i)) << (2*i + 2))
        ;
    }
    return ret ;
  }

  int32_t
  _morton( _rgba_type const&o )
  {
    return _morton( o.r, o.g, o.b );
  }

  int32_t
  _morton( int32_t n )
  {
    return _morton( _rgba( n ) );
  }
}

int
main(int,char**)
{
  assert( 67845 == _int32( 5, 9, 1 ) );
  assert(  1095 == _morton( 5, 9, 1 ) );
  assert(  1095 == _morton( _int32( 5, 9, 1 ) ) );

  std::cout
    << std::dec
    << "_int32( 5, 9, 1 ) == "
    << _int32( 5, 9, 1 )
    << std::endl
    ;

  std::cout
    << std::dec
    << "_morton( 5, 9, 1 ) == "
    << _morton( 5, 9, 1 )
    << std::endl
    ;

  std::cout
    << std::dec
    << "_morton( _int32( 5, 9, 1 ) ) == "
    << _morton( _int32( 5, 9, 1 ) )
    << std::endl
    ;

  return EXIT_SUCCESS ;
}
