/* octree/octree.cc */

#include <cstddef>
#include <cstdint>
#include <type_traits>

#if HAVE_CONFIG_H
#  include "config.h"
#else
#  error "missing config.h"
#endif

#include "octree.hh"


#define MY_STATIC_ASSERT( EXPR ) static_assert((EXPR),#EXPR)


namespace octree
{
  struct node
  {
    node*parent_;
    node*children_;
    uint32_t leaf_  : 8 ;
    uint32_t value_ : 24 ;
    node(node*parent, uint32_t v)
      :parent_(parent)
      ,children_(nullptr)
      ,leaf_(1)
      ,value_(0xffffff & v)
    {}
    node(node*parent)
      :parent_(parent)
      ,children_(nullptr)
      ,leaf_(0)
      ,value_(0)
    {}
    node(void):node{nullptr}{}
  };
  MY_STATIC_ASSERT(std::is_standard_layout< node >::value);
}

namespace
{
  using ::octree::node ;

  uint32_t _value( node*o )
  {
    //assert(o)
    //assert(_is_leaf(o))
    return o->value_ ;
  }

  node*_children     ( node*o ) { return o->children_ ; }
  bool _has_children ( node*o ) { return _children(o) ; }

  uint8_t _leaf ( node*o ) { return o->leaf_     ; }
  bool _is_leaf ( node*o ) { return 1 & _leaf(o) ; }


  bool _empty( node*o )
  {
    if( o ){
      if( _is_leaf      (o) ) return false ;
      if( _has_children (o) ) return false ;
    }
    return true ;
  }


  int _weight_recursive( node*o )
  {
    //assert(o)
    if( _is_leaf(o) ) return 1 ;

    node*children = _children(o);
    if( ! children ) return 0 ;

    int acc = 0 ;
    for( size_t i = 0 ; i != 8 ; ++i ){
      acc += _weight_recursive( children + i );
    }
    return acc ;
  }
  int _weight( node*o ){ return _weight_recursive( o ); }
  

    
  void
  _destroy_recursive( node*o )
  {
    node*children = _children(o);
    if(children){
      for( size_t i = 0 ; i < 8 ; ++i ){
        _destroy_recursive( children + i );
      }
      delete[]children;
    }
    if( ! o->parent_ ) delete o ;
  }

  /* take a node, value, and depth (zero means root) */
  node*
  _insert( node*o, int const v, size_t const d )
  {
    //assert(o);
    /* at level 0, shift 21 bits */
    /* at level 7, shift  0 bits */
    size_t const idx = 7 & ( v >> ((7-d)*3) ) ;

    if( _is_leaf(o) ){
      /* leaf node */
      return nullptr ; // todo
    }else{
      node*children = _children(o);
      if(children){
        /* interior node */
        return _insert( children + idx, v, d+1 );
      }else{
        /* unused node */
        o->leaf_ = 1 ;
        o->value_ = 0xffffff & v ;
        return o ;
      }
    }
  }
}

node*octree::init(void){ return new node ; }

void octree::destroy( node*o ){ if(!_empty(o)) _destroy_recursive(o); }

int octree::value( node*o ){ return o && _is_leaf(o) ? _value(o) : -1 ; }

int octree::weight( node*o ){ return _empty(o) ? 0 : _weight(o); }

node*octree::insert( node*o, int v )
{
  return o ? nullptr : _insert(o,v,0) ;
}




#if 0

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
