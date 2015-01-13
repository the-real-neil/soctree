/* octree/octree.cc */

#include "octree.hh"

#include <cstddef>


octree::node_t*
octree::create( void )
{
    octree::node_t*ret = new octree::node_t();
    ret->parent_ = NULL ;
    for( size_t i = 0 ; i != 8 ; ++i )
        ret->children_[i] = NULL ;
    ret->value_ = 0.0 ;
    return ret ;
}


void
octree::destroy( node_t*o )
{
    if( NULL == o ) return ;
    for( size_t i = 0 ; i != 8 ; ++i ){
        octree::destroy( o->children_[i] );
    }
    delete o ;
    o = NULL ;
}


octree::node_t*
octree::insert( octree::node_t*, int, int, int, float )
{
    return NULL ;
}


octree::node_t*
octree::remove( node_t*, int, int, int )
{
    return NULL ;
}
