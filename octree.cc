/* octree/octree.cc */

#include "octree.hh"

#include <cstddef>

namespace
{
  using ::octree::node ;

  typedef unsigned char uchar ;


  void
  _destroy( node*o )
  {
    if( NULL == o ) return ;
    _destroy( o->children_[0][0][0] );
    _destroy( o->children_[0][0][1] );
    _destroy( o->children_[0][1][0] );
    _destroy( o->children_[0][1][1] );
    _destroy( o->children_[1][0][0] );
    _destroy( o->children_[1][0][1] );
    _destroy( o->children_[1][1][0] );
    _destroy( o->children_[1][1][1] );
    delete o ;
    o = NULL ;
  }

  node*
  _create( node*parent )
  {
    node*ret = new node();
    ret->parent_ = parent ;
    ret->children_[0][0][0] = NULL ;
    ret->children_[0][0][1] = NULL ;
    ret->children_[0][1][0] = NULL ;
    ret->children_[0][1][1] = NULL ;
    ret->children_[1][0][0] = NULL ;
    ret->children_[1][0][1] = NULL ;
    ret->children_[1][1][0] = NULL ;
    ret->children_[1][1][1] = NULL ;
    return ret ;
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
  return _create( NULL );
}

void
octree::destroy( node*o )
{
  _destroy( o );
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
