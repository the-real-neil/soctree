/* github.com/rubicks/soctree/soctree.h */

#ifndef ___SOCTREE___SOCTREE_H___
#define ___SOCTREE___SOCTREE_H___

#ifdef __cplusplus
extern "C" {
#endif

/* create and return the root node of an empty tree */
void* soctree_init(void);

/* recursively destroy the given node */
void soctree_free(void*);

/* given a root, create (if necessary) and return the specified node */
void* soctree_insert(void*, int);

/* given a root, remove the specified node if it exists */
void* soctree_remove(void*, int);

/* given a root, return the specified node if it exists, NULL otherwise */
void* soctree_find(void*, int);

/* take a node, return the return the weight contained */
int soctree_weight(void*);

/* take a node, return the depth contained */
int soctree_depth(void*);

/* take a node, return its value */
int soctree_value(void*);

#ifdef __cplusplus
}
#endif

#endif  // ndef ___SOCTREE___SOCTREE_H___
