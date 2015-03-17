/* octree/octree.hh */

#ifndef ___OCTREE___OCTREE_HH___
#define ___OCTREE___OCTREE_HH___


namespace octree
{
  struct node ;

  /* create and return the root node of an empty tree */
  node*init( void );

  /* recursively destroy the given node */
  void destroy( node* );

  /* given a root, create (if necessary) and return the specified node */
  node*insert( node*, int );

  /* given a root, remove the specified node if it exists */
  void remove( node*, int );

  /* given a root, return the specified node if it exists, NULL otherwise */
  node*get( node*, int );

  /* take a node, return its value */
  int value( node* );

  /* take a node, return the weight contained */
  int weight( node* );

  /* take a node, return the depth contained */
  int depth( node* );
}


#endif//ndef ___OCTREE___OCTREE_HH___
