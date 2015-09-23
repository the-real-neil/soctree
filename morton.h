/* github.com/rubicks/soctree/morton.h */

#ifndef ___SOCTREE___MORTON_H___
#define ___SOCTREE___MORTON_H___

#ifdef __cplusplus
extern "C" {
#endif

unsigned soctree_morton_expand(unsigned);
unsigned soctree_morton_shrink(unsigned);
unsigned soctree_morton_encode(unsigned);
unsigned soctree_morton_decode(unsigned);

#ifdef __cplusplus
}
#endif

#endif  // ndef ___SOCTREE___MORTON_H___
