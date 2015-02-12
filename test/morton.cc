/* octree/test/morton.cc */

#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <type_traits>

#include <limits.h>

#if HAVE_CONFIG_H
#  include "config.h"
#else
#  error "missing config.h"
#endif

#include "binlit.hh"
#include "morton.hh"


namespace
{
  struct _rgba_type
  {
#if 0
    /*  */
#elif IS_LITTLE_ENDIAN
    uint8_t r ;
    uint8_t g ;
    uint8_t b ;
    uint8_t a ;
#elif IS_BIG_ENDIAN
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

#if 0
  _rgba_type
  _rgba( int32_t n )
  {
    _rgba_union un ;
    un.int32_ = n ;
    return un.rgba_ ;
  }
#endif


#if 0
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
#endif

  /* returns the largest power of two that does not exceed the given number */
  unsigned long _floor2( unsigned long v )
  {
    unsigned long ret ;
    do{
      ret = v ;
    }while( v&=v-1 );
    return ret ;
  }

  unsigned long _cardinality( unsigned long v )
  {
    unsigned long c = 0 ;
    for( ; v ; ++c ) v&=v-1 ;
    return c ;
  }

  unsigned
  _part3( unsigned v )
  {
    v &= 0xffffff ;
    v = (v|(v<<16)) & (unsigned)000000000000000011111111_b ; // 0x0000ff
    v = (v|(v<< 8)) & (unsigned)000000001111000000001111_b ; // 0x00f00f
    v = (v|(v<< 4)) & (unsigned)000011000011000011000011_b ; // 0x0c30c3
    v = (v|(v<< 2)) & (unsigned)001001001001001001001001_b ; // 0x249249
    return v ;
  }


#if 0
  uint32_t
  _morton( uint8_t r, uint8_t g, uint8_t b )
  {
    uint32_t ret = 0 ;
#if 0
    for( size_t i = 0 ; i < 8 ; ++i ){
      ret |= 0
        | ((r & ((uint8_t)1 << i)) << (2*i + 0))
        | ((g & ((uint8_t)1 << i)) << (2*i + 1))
        | ((b & ((uint8_t)1 << i)) << (2*i + 2))
        ;
    }
#else
    ret |= _part3( b );
    ret <<= 1 ;
    ret |= _part3( g );
    ret <<= 1 ;
    ret |= _part3( r );
    /* ret |= _part3( r ) << 0 ; */
    /* ret |= _part3( g ) << 1 ; */
    /* ret |= _part3( b ) << 2 ; */
#endif
    return ret ;
  }

  uint32_t
  _morton( _rgba_type const&o )
  {
    return _morton( o.r, o.g, o.b );
  }

  template< typename T >
  uint32_t
  _morton( T n )
  {
    static_assert( std::is_integral< T >::value, "integral type required" );
    return _morton( _rgba( uint32_t( n ) ) );
  }
#endif



#define N_BITS(v) (sizeof(v)*CHAR_BIT)

  
  std::string
  _bitstr( unsigned v )
  {
    std::string ret = "" ;
    do{
      ret = ( ( 1 & v ) ? "1" : "0" ) + ret ;
    }while( v >>=1 );
    /* for( unsigned m = 1 << _log2(v) ; m ; m >>=1 ){ */
    /*   ret += ( v & m ) ? "1" : "0" ; */
    /* } */
    return ret + "b" ;
  }
}

int
main(int,char**)
{

  assert( 0 == 000_b );
  assert( 1 == 001_b );
  assert( 2 == 010_b );
  assert( 3 == 011_b );
  assert( 4 == 100_b );
  assert( 5 == 101_b );
  assert( 6 == 110_b );
  assert( 7 == 111_b );

  assert( 32 == 100000_b );
  assert( 42 == 101010_b );
  assert( 32 == _floor2( 42 ) );
  assert( 100000_b == _floor2( 101010_b ) );

  assert( 3 == _cardinality( 101010_b ) );
  assert( 3 == _cardinality( 101001_b ) );
  assert( 3 == _cardinality( 100101_b ) );
  assert( 3 == _cardinality( 100011_b ) );

  assert( "111b" == _bitstr(7));
  assert( "101010b" == _bitstr(42));
  assert( "11110000111100001111b" == _bitstr(0x0f0f0f));

  assert( "11111111b"               == _bitstr(0x0000ff) );
  assert( "1111000000001111b"       == _bitstr(0x00f00f) );
  assert( "11000011000011000011b"   == _bitstr(0x0c30c3) );
  assert( "1001001001001001001001b" == _bitstr(0x249249) );

  {
    using morton::encode ;
    typedef unsigned un ;
    assert(000000000000000000000000_b==encode((un)000000000000000000000000_b));
    assert(000000000000001001001001_b==encode((un)000000000000000000001111_b));
    assert(001001001001000000000000_b==encode((un)000000000000000011110000_b));
    assert(001001001001001001001001_b==encode((un)000000000000000011111111_b));

    assert(000000000000010010010010_b==encode((un)000000000000111100000000_b));
    assert(000000000000011011011011_b==encode((un)000000000000111100001111_b));
    assert(001001001001010010010010_b==encode((un)000000000000111111110000_b));
    assert(001001001001011011011011_b==encode((un)000000000000111111111111_b));

    assert(010010010010010010010010_b==encode((un)000000001111111100000000_b));
    assert(011011011011011011011011_b==encode((un)000000001111111111111111_b));

    assert(100100100100100100100100_b==encode((un)111111110000000000000000_b));
    assert(101101101101101101101101_b==encode((un)111111110000000011111111_b));
    assert(110110110110110110110110_b==encode((un)111111111111111100000000_b));

    assert(111111111111111111111111_b==encode((un)111111111111111111111111_b));
  }

  assert( 0x249249 == _part3( 0xff ) );

  return EXIT_SUCCESS ;
}
