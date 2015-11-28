/* github.com/rubicks/soctree/soctree.c */

#include "soctree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_CONFIG_H
#include "config.h"
#else
#error "missing config.h"
#endif

#define UINT24(V) ((V) & ((uint32_t)0xffffff))

/***************************************************************/
/* http://www.pixelbeat.org/programming/gcc/static_assert.html */
/***************************************************************/
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
/* These can't be used after statements in c89. */
#ifdef __COUNTER__
#define STATIC_ASSERT(e, m) \
  enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1 / (!!(e)) }
#else
/* This can't be used twice on the same line so ensure if using in headers
 * that the headers are not included twice (by wrapping in #ifndef...#endif)
 * Note it doesn't cause an issue when used on same line of separate modules
 * compiled with gcc -combine -fwhole-program.  */
#define STATIC_ASSERT(e, m) \
  enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1 / (!!(e)) }
#endif

#define max(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })

#if 0
#define SPIT(OSTREAM)                                                     \
  do {                                                                    \
    OSTREAM << __PRETTY_FUNCTION__ << "; " << __FILE__ << ":" << std::dec \
            << __LINE__ << std::endl;                                     \
  } while (0) /**/
#else
#define SPIT(_) \
  do {          \
  } while (0)
#endif

typedef struct _node {
  struct _node *parent_;
  union {
    struct _node *children_;
    struct {
      uint32_t lo08_ : 8;
      uint32_t hi24_ : 24;
    } leaf_;
  } un_;
} _node_t;

#define CHILDREN(PTR) ((PTR)->un_.children_)
#define LEAF(PTR) ((PTR)->un_.leaf_.lo08_)
#define VALUE(PTR) ((PTR)->un_.leaf_.hi24_)

STATIC_ASSERT(sizeof(_node_t) == 2 * sizeof(void *), "fail");

static bool _is_aligned(void *ptr) {
  return !((uintptr_t)ptr % sizeof(_node_t));
}

static _node_t *_validate(void *ptr) {
  return ptr && _is_aligned(ptr) ? ptr : NULL;
}

static _node_t *_children(_node_t *o) { return CHILDREN(o); }

static bool _has_children(_node_t *o) { return _validate(_children(o)); }

static bool _is_leaf(_node_t *o) { return 1 & o->un_.leaf_.lo08_; }

static void _leaf_set(_node_t *o) { o->un_.leaf_.lo08_ |= 1; }

static void _leaf_clear(_node_t *o) { o->un_.leaf_.lo08_ &= (uint8_t)~1; }

static void _value_set(_node_t *o, uint32_t v) { VALUE(o) = UINT24(v); }

static int _weight_recursive(_node_t *o) {
  if (_is_leaf(o)) {
    return 1;
  }
  if (!_has_children(o)) {
    return 0;
  }
  int acc = 0;
  for (size_t i = 0; i != 8; ++i) {
    acc += _weight_recursive(_children(o) + i);
  }
  return acc;
}

static int _weight(_node_t *o) { return _weight_recursive(o); }

#if 0
static int _depth_recursive(_node_t *o) {
  SPIT(std::cout << std::endl);

  if (_is_leaf(o)) return 0;

  if (!_has_children(o)) return 0;

  int acc = 0;
  for (size_t i = 0; i != 8; ++i) {
    acc = _imax(acc, _depth_recursive(_children(o) + i));
  }
  return 1 + acc;
}

static int _depth(_node_t *o) { return _depth_recursive(o); }
#endif

void _delete_recursive(_node_t *o) {
  if (_has_children(o)) {
    _node_t *children = _children(o);
    for (size_t i = 0; i < 8; ++i) {
      _delete_recursive(children + i);
    }
    free(children);
  }
}

size_t _idx8(uint32_t v, size_t d) { return (7 & (v >> ((7 - d) * 3))); }

_node_t *_children_new(void) {
  size_t n = 8 * sizeof(_node_t);
  _node_t *ret = (_node_t *)malloc(n);
  return memset(ret, 0, n);
}

/* take a _node_t, value, and depth (zero means root) */
_node_t *_insert(_node_t *o, uint32_t v, size_t d) {
  /* at level 0, shift 21 bits */
  /* at level 7, shift  0 bits */

  if (_has_children(o)) {
    SPIT(std::cout);
    return _insert(CHILDREN(o) + _idx8(v, d), v, d + 1);
  }

  if (!_is_leaf(o)) {
    /* unused _node_t */
    _leaf_set(o);
    _value_set(o, v);
    return o;
  }

  /* else occupied leaf */

  if (UINT24(v) == VALUE(o)) {
    /* same value */
    return o;
  }

  /* else different value */

  _leaf_clear(o);

  uint32_t w = VALUE(o);  // save old value
  VALUE(o) = 0;           // clear value
  CHILDREN(o) = _children_new();
  for (size_t i = 0; i < 8; ++i) {
    (CHILDREN(o) + i)->parent_ = o;
  }

  /* insert old value */
  _insert(_children(o) + _idx8(w, d), w, d + 1);

  /* recurse with given value */
  return _insert(_children(o) + _idx8(v, d), v, d + 1);
}

/* take a _node_t, value, and depth (zero means root) */
_node_t *_find(_node_t *o, uint32_t v, size_t d) {
  if (_is_leaf(o)) {
    return VALUE(o) == v ? o : NULL;
  }
  if (!_has_children(o)) {
    return NULL;
  }
  return _find(_children(o) + _idx8(v, d), v, d + 1);
}

struct soctree {
  _node_t *root_;
};

soctree_t *soctree_new(void) {
  soctree_t *ret = (soctree_t *)malloc(sizeof(soctree_t));
  _node_t *root = (_node_t *)malloc(sizeof(_node_t));
  if (ret && root) {
    ret->root_ = memset(root, 0, sizeof(_node_t));
    return ret;
  }
  free(root);
  free(ret);
  return NULL;
}

/* guard predicate */
_node_t *_root(soctree_t *p) {
  if (p) {
    if (0 == (uintptr_t)p % sizeof(soctree_t)) {
      return _validate(p->root_);
    }
  }
  return NULL;
}

void soctree_delete(soctree_t *p) {
  _node_t *root = _root(p);
  if (root) {
    _delete_recursive(root);
    free(root);
    root = 0;
  }
  free(p);
}

int soctree_insert(soctree_t *p, int w) {
  _node_t *root = _root(p);
  if (!root) {
    return -1;
  }
  if (0 > w) {
    return -1;
  }
  if (0xffffff < w) {
    return -1;
  }
  _insert(root, 0xffffff & w, 0);
  return 0;
}

#define UNUSED(X) (void)((X))

int soctree_erase(soctree_t *p, int w) {
  /* todo */
  UNUSED(p);
  UNUSED(w);
  return -1;
}

int soctree_find(soctree_t *p, int w) {
  _node_t *root = _root(p);
  if (!root) {
    goto fail;
  }
  if (0 > w) {
    goto fail;
  }
  if (0xffffff < w) {
    goto fail;
  }
  _node_t *found = _find(root, 0xffffff & w, 0);
  if (!found) {
    goto fail;
  }
  return 0;
fail:
  return -1;
}

int soctree_size(soctree_t *p) {
  _node_t *root = _root(p);
  return root ? _weight(root) : -1;
}
