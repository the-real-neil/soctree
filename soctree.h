/* github.com/rubicks/soctree/soctree.h */

#ifndef ___SOCTREE___SOCTREE_H___
#define ___SOCTREE___SOCTREE_H___

#ifdef __cplusplus
extern "C" {
#endif

typedef struct soctree soctree_t;

/* create and initialize an empty soctree; return pointer on success, NULL on
 * failure */
soctree_t* soctree_new(void);

/* release the given soctree */
void soctree_delete(soctree_t*);

/* insert into the given soctree a copy of the given value; return 0 on success,
 * negative on failure */
int soctree_insert(soctree_t*, int);

/* erase from the given soctree a copy of the given value; return 0 on success,
 * negative on failure */
int soctree_erase(soctree_t*, int);

/* search the given soctree for the given value; return 0 on success, negative
 * on failure */
int soctree_find(soctree_t*, int);

/* get the size of the given soctree; return non-negative 24-bit value on
 * success, negative on failure */
int soctree_size(soctree_t*);

#ifdef __cplusplus
}
#endif

#endif  // ndef ___SOCTREE___SOCTREE_H___
