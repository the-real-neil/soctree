/* github.com/rubicks/soctree/soctree.cc */

#include <cassert>
#include <cstdint>
#include <iostream>

#if HAVE_CONFIG_H
#include "config.h"
#else
#error "missing config.h"
#endif

#include "soctree.h"

#define UINT24(V) ((0xffffff & (V)))

#define VAL(NODEPTR) (UINT24((NODEPTR)->value_))

#define MY_STATIC_ASSERT(EXPR) static_assert((EXPR), #EXPR)

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

namespace soctree {
  struct node {
    node *parent_;
    node *children_;
    uint32_t leaf_ : 8;
    uint32_t value_ : 24;
    node(node *parent, uint32_t v)
        : parent_(parent), children_(nullptr), leaf_(1), value_(UINT24(v)) {}
    node(node *parent)
        : parent_(parent), children_(nullptr), leaf_(0), value_(0) {}
    node(void) : node{nullptr} {}
  };
  MY_STATIC_ASSERT(std::is_standard_layout<node>::value);
}

namespace {
  template <typename T>
  bool _is_aligned(T const *const p, size_t const n = alignof(T)) {
    return 0 == reinterpret_cast<uintptr_t>(p) % n;
  }

  using ::soctree::node;

  bool _is_valid(node *p) { return nullptr != p && _is_aligned(p); }

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

  node *_children(node *o) { return o->children_; }

  bool _has_children(node *o) {
    node const *const children(_children(o));
    return children && _is_aligned(children);
  }

  uint8_t _leaf(node *o) { return o->leaf_; }

  bool _is_leaf(node *o) { return 1 & _leaf(o); }

  int _weight_recursive(node *o) {
    SPIT(std::cout << std::endl);

    if (_is_leaf(o)) return 1;

    if (!_has_children(o)) return 0;

    int acc = 0;
    for (size_t i = 0; i != 8; ++i) {
      acc += _weight_recursive(_children(o) + i);
    }
    return acc;
  }
  int _weight(node *o) { return _weight_recursive(o); }

  int _depth_recursive(node *o) {
    SPIT(std::cout << std::endl);

    if (_is_leaf(o)) return 0;

    if (!_has_children(o)) return 0;

    int acc = 0;
    for (size_t i = 0; i != 8; ++i) {
      acc = std::max(acc, _depth_recursive(_children(o) + i));
    }
    return 1 + acc;
  }
  int _depth(node *o) { return _depth_recursive(o); }

  void _destroy_recursive(node *o) {
    SPIT(std::cout << std::endl);
    if (_has_children(o)) {
      node *children = _children(o);
      for (size_t i = 0; i < 8; ++i) {
        _destroy_recursive(children + i);
      }
      delete[] children;
    }
    if (!o->parent_) delete o;
  }

  size_t _idx8(int const v, size_t const d) {
    return (7 & (v >> ((7 - d) * 3)));
  }

  /* take a node, value, and depth (zero means root) */
  node *_insert(node *o, int const v, size_t const d) {
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
      /* unused node */
      o->leaf_ = 1;
      o->value_ = UINT24(v);
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

    o->children_ = new node[8];  // allocate children
    for (size_t i = 0; i != 8; ++i) {
      (o->children_ + i)->parent_ = o;
    }

    /* insert old value */
    _insert(_children(o) + _idx8(w, d), w, d + 1);

    /* recurse with given value */
    return _insert(_children(o) + _idx8(v, d), v, d + 1);
  }

}  // end anonymous namespace

int soctree_value(void *ptr) {
  node *o = (node *)ptr;
  if (_is_valid(o)) {
    if (_is_leaf(o)) {
      return o->value_;
    }
  }
  return -1;
}

void *soctree_init(void) { return new node; }

void soctree_destroy(void *ptr) {
  node *o = (node *)ptr;
  if (_is_valid(o)) _destroy_recursive(o);
}

int soctree_weight(void *ptr) {
  node *o = (node *)ptr;
  return _is_valid(o) ? _weight(o) : 0;
}

int soctree_depth(void *ptr) {
  node *o = (node *)ptr;
  return _is_valid(o) ? _depth(o) : 0;
}

void *soctree_insert(void *ptr, int v) {
  SPIT(std::cout << std::endl);
  node *o = (node *)ptr;
  return _is_valid(o) ? _insert(o, v, 0) : nullptr;
}

#if 0

node*
soctree::get( node*root, uchar r, uchar g, uchar b )
{
  return _get( root, r, g, b );
}

void
soctree::remove( node*root, uchar r, uchar g, uchar b )
{
  _remove( root, r, g, b );
}

#endif
