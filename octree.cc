/* octree/octree.cc */

#include <cstddef>
#include <cstdint>

#if HAVE_CONFIG_H
#  include "config.h"
#else
#  error "missing config.h"
#endif

#include "octree.hh"


#define MY_STATIC_ASSERT( EXPR ) static_assert((EXPR),#EXPR)

namespace
{
  struct leaf
  {
    uint32_t lsb_ :  1 ;
    uint32_t pad_ :  7 ;
    uint32_t val_ : 24 ;
  };
  MY_STATIC_ASSERT(sizeof(leaf)==sizeof(uint32_t));

  using ::octree::node ;

  union un
  {
    node*children_;
    leaf leaf_;
  };
  MY_STATIC_ASSERT(sizeof(un)==sizeof(void*));
  MY_STATIC_ASSERT(sizeof(un)>=sizeof(leaf));
}

namespace octree
{
  struct node
  {
    node*parent_;
    un un_ ;
  };
  MY_STATIC_ASSERT(sizeof(node)==2*sizeof(node*));
}

namespace
{
  using ::octree::node ;

  typedef unsigned char uchar ;

  bool _ischild( node*o ){ return o->un_.leaf_.lsb_ ; }

  void _destroy( node*o )
  {
    if( nullptr == o ) return ;
    if( ! _ischild( o ) ){
      for( size_t i = 0 ; i != 8 ; ++i ){
        _destroy( &(o->un_.children_[0]) );
      }
    }
    delete o ;
    o = nullptr ;
  }

  node*_create( node*parent, uint32_t val )
  {
    node*ret            = new node()     ;
    ret->parent_        = parent         ;
    ret->un_.children_  = nullptr        ;
    ret->un_.leaf_.lsb_ = 1              ;
    ret->un_.leaf_.pad_ = 0              ;
    ret->un_.leaf_.val_ = 0xffffff & val ;
    return ret ;
  }
  node*_create( node*parent ){ return _create( parent, (uint32_t)0 ); }
  node*_create( void ){ return _create( nullptr ); }

  unsigned
  _value( node*o )
  {
    if( ! _ischild( o ) ) return 0 ;
    return o->un_.leaf_.val_ ;
  }


#if 0

  node*
  _insert( node*root, int val )
  {
    node*ret = root ;
    if( NULL == ret ) return ret ;
    size_t n = 8 ;
    do{
      --n ;
      node*child ;
      ret->children_
        [ ( r >> n ) & 1 ]
        [ ( g >> n ) & 1 ]
        [ ( b >> n ) & 1 ] = child = _create( ret );
      ret = child ;
    }while( n );
    return ret ;
  }

  node*
  _child( size_t n, node*o, uchar r, uchar g, uchar b )
  {
    return
      o->children_
      [ ( r >> n ) & 1 ]
      [ ( g >> n ) & 1 ]
      [ ( b >> n ) & 1 ] ;
  }

  node*
  _get( node*root, uchar r, uchar g, uchar b )
  {
    node*ret = root ;
    size_t n = 8 ;
    while( NULL != ret && 0 < n ){
      --n ;
      node*ret = _child( n, ret, r, g, b );
    }
    return ret ;
  }

  void
  _remove( node*, uchar, uchar, uchar )
  {
    
  }
#endif
}


node*
octree::init( void )
{
  return _create();
}

void
octree::destroy( node*o )
{
  _destroy( o );
}

unsigned
octree::value( node*o )
{
  if( nullptr == o ) return 0 ;
  return _value( o );
}

#if 0

node*
octree::insert( node*root, uchar r, uchar g, uchar b )
{
  return _insert( root, r, g, b );
}

node*
octree::get( node*root, uchar r, uchar g, uchar b )
{
  return _get( root, r, g, b );
}

void
octree::remove( node*root, uchar r, uchar g, uchar b )
{
  _remove( root, r, g, b );
}

#endif
