/* github.com/rubicks/soctree/soctree.c */

#include "soctree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if HAVE_CONFIG_H
#include "config.h"
#else
#error "missing config.h"
#endif

#define UINT24(V) ((0xffffff & (V)))

#define VAL(NODEPTR) (UINT24((NODEPTR)->value_))

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

STATIC_ASSERT(sizeof(_node_t) == 2 * sizeof(void *), "fail");

#if 0
template <typename T>
bool _is_aligned(T const *const p, size_t const n = alignof(T)) {
  return 0 == reinterpret_cast<uintptr_t>(p) % n;
}
#endif

static int _imax(int a, int b) { return a < b ? b : a; }

static bool _is_aligned(void *ptr) {
  return !((uintptr_t)ptr % sizeof(_node_t));
}

static _node_t *_validate(void *ptr) {
  return ptr && _is_aligned(ptr) ? ptr : NULL;
}

#if 0
  std::ostream&
  operator<<( std::ostream&os, node const&x )
  {
    return
      os
      << "{"
      << "parent_:" << x.parent_
      << ","
      << "children_:" << x.children_
      << ","
      << "leaf_:" << x.leaf_
      << ","
      << "value_:" << x.value_
      << "}"
      ;
  }
#endif

static _node_t *_children(_node_t *o) { return o->un_.children_; }

static bool _has_children(_node_t *o) { return _validate(_children(o)); }

static bool _is_leaf(_node_t *o) { return 1 & o->un_.leaf_.lo08_; }

static void _leaf_set(_node_t *o) { o->un_.leaf_.lo08_ |= 1; }

static void _leaf_clear(_node_t *o) { o->un_.leaf_.lo08_ &= ~1; }

static void _value_set(_node_t *o, int v) { o->un_.leaf_.hi24_ = UINT24(v); }

static int _weight_recursive(_node_t *o) {
  SPIT(std::cout << std::endl);

  if (_is_leaf(o)) return 1;

  if (!_has_children(o)) return 0;

  int acc = 0;
  for (size_t i = 0; i != 8; ++i) {
    acc += _weight_recursive(_children(o) + i);
  }
  return acc;
}

static int _weight(_node_t *o) { return _weight_recursive(o); }

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

void _destroy_recursive(_node_t *o) {
  SPIT(std::cout << std::endl);
  if (_has_children(o)) {
    _node_t *children = _children(o);
    for (size_t i = 0; i < 8; ++i) {
      _destroy_recursive(children + i);
    }
    free(children);
  }
  if (!o->parent_) free(o);
}

size_t _idx8(int const v, size_t const d) { return (7 & (v >> ((7 - d) * 3))); }

/* take a _node_t, value, and depth (zero means root) */
_node_t *_insert(_node_t *o, int v, size_t d) {
  SPIT(std::cout << std::endl);

  // assert(o);
  /* at level 0, shift 21 bits */
  /* at level 7, shift  0 bits */

  if (_has_children(o)) {
    SPIT(std::cout);
    return _insert(_children(o) + _idx8(v, d), v, d + 1);
  }

  if (!_is_leaf(o)) {
    SPIT(std::cout << std::endl);
    /* unused _node_t */
    _leaf_set(o);
    _value_set(o, v);
    return o;
  }

  /* else occupied leaf */

  if (UINT24(v) == o->value_) {
    /* same value */
    return o;
  }

  /* else different value */

  o->leaf_ = 0;  // clear leaf bit

  int w = o->value_;  // save old value

  o->value_ = 0;  // clear value

  o->children_ = new _node_t[8];  // allocate children
  for (size_t i = 0; i != 8; ++i) {
    (o->children_ + i)->parent_ = o;
  }

  /* insert old value */
  _insert(_children(o) + _idx8(w, d), w, d + 1);

  /* recurse with given value */
  return _insert(_children(o) + _idx8(v, d), v, d + 1);
}

int soctree_value(void *ptr) {
  _node_t *o = (_node_t *)ptr;
  if (_is_valid(o)) {
    if (_is_leaf(o)) {
      return o->value_;
    }
  }
  return -1;
}

void *soctree_init(void) { return new _node_t; }

void soctree_destroy(void *ptr) {
  _node_t *o = (_node_t *)ptr;
  if (_is_valid(o)) _destroy_recursive(o);
}

int soctree_weight(void *ptr) {
  _node_t *o = (_node_t *)ptr;
  return _is_valid(o) ? _weight(o) : 0;
}

int soctree_depth(void *ptr) {
  _node_t *o = (_node_t *)ptr;
  return _is_valid(o) ? _depth(o) : 0;
}

void *soctree_insert(void *ptr, int v) {
  SPIT(std::cout << std::endl);
  _node_t *o = (_node_t *)ptr;
  return _is_valid(o) ? _insert(o, v, 0) : nullptr;
}

#if 0

_node_t*
soctree::get( _node_t*root, uchar r, uchar g, uchar b )
{
  return _get( root, r, g, b );
}

void
soctree::remove( _node_t*root, uchar r, uchar g, uchar b )
{
  _remove( root, r, g, b );
}

#endif
