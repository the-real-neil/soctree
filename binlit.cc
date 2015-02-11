/* octree/binlit.cc */


#include <limits.h>
#include <cassert>
#include <iostream>

#include "binlit.hh"

long unsigned
operator""_b( char const*p )
{
  long unsigned ret = 0 ;
  while( *p ){
    ret <<= 1 ;
    ret |= *p++-'0' ;
  }
  return ret ;
}

