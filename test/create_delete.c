/* soctree/test/create_delete.c */

#include "soctree.h"

#include <assert.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  soctree_t* s = soctree_new();

  assert(NULL != s);
  assert(0 == soctree_size(s));

  /* empty soctree find fail */
  assert(-1 == soctree_find(s, 0));

  /* first insert */
  assert(0 == soctree_insert(s, 0));
  assert(1 == soctree_size(s));
  assert(0 == soctree_find(s, 0));

  /* second insert */
  assert(0 == soctree_insert(s, 0xffffff));
  assert(2 == soctree_size(s));
  assert(0 == soctree_find(s, 0xffffff));

  /* dupe insert */
  assert(0 == soctree_insert(s, 0));
  assert(2 == soctree_size(s));
  assert(0 == soctree_find(s, 0));

  soctree_delete(s);
  s = 0;

  return EXIT_SUCCESS;
}
