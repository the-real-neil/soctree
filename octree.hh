/* octree/octree.hh */

#ifndef ___OCTREE___OCTREE_HH___
#define ___OCTREE___OCTREE_HH___


namespace octree
{
  struct node
  {
    node*parent_ ;
    node*children_[2][2][2] ;
  };

  /* create and return a parent-less (root) node */
  node*init( void );

  /* recursively destroy the given node */
  void destroy( node* );

  /* given a root, create (if necessary) and return the specified node */
  node*insert( node*, int );

  /* given a root, return the specified node if it exists, NULL otherwise */
  node*get( node*, int );

  /* given a root, remove the specified node if it exists */
  void remove( node*, int );
}


#endif//ndef ___OCTREE___OCTREE_HH___
