/* octree/morton.cc */


#if HAVE_CONFIG_H
#  include "config.h"
#else
#  error "missing config.h"
#endif

#include "morton.hh"


namespace
{
  int
  _encode( int n )
  {
    return n ;
  }
  
  int
  _decode( int n )
  {
    return n ;
  }
}



int morton::encode( int n ){ return _encode( n ); }

int morton::decode( int n ){ return _decode( n ); }
