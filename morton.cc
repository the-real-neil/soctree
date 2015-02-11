/* octree/morton.cc */


#if HAVE_CONFIG_H
#  include "config.h"
#else
#  error "missing config.h"
#endif

#include "morton.hh"

#include <limits.h>
#include <stdint.h>


namespace
{
  unsigned
  _part3( unsigned v )
  {
    v &= 0xffffff ;
    v = (v|(v<<16)) & 0x0000ff ;
    v = (v|(v<< 8)) & 0x00f00f ;
    v = (v|(v<< 4)) & 0x0c30c3 ;
    v = (v|(v<< 2)) & 0x249249 ;
    return v ;
  }

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

  _rgba_type
  _rgba( int32_t n )
  {
    _rgba_union un ;
    un.int32_ = n ;
    return un.rgba_ ;
  }

  unsigned
  _encode( uint8_t r, uint8_t g, uint8_t b )
  {
    unsigned ret = 0 ;
    ret |= _part3( r ) << 0 ;
    ret |= _part3( g ) << 1 ;
    ret |= _part3( b ) << 2 ;
    return ret ;
  }
  
  unsigned
  _encode( _rgba_type const&o )
  {
    return _encode( o.r, o.g, o.b );
  }



  unsigned
  _decode( unsigned n )
  {
    return n ;
  }
}



unsigned morton::encode( unsigned n ){ return _encode( _rgba( n ) ); }

unsigned morton::decode( unsigned n ){ return _decode( n ); }
