/* octree/test/create_delete.cc */

#include "soctree.h"

#include <assert.h>
#include <stdlib.h>

int main(int, char**) {
  using namespace octree;

  node* o = nullptr;

  assert(nullptr == o);
  assert(0 == weight(o));
  assert(-1 == value(o));

  o = init();

  /* init should return non-null ptr to unused root node */
  assert(nullptr != o);
  assert(0 == weight(o));
  assert(-1 == value(o));
  assert(0 == depth(o));

  /* first insert should fill root node */
  assert(o == insert(o, 0));
  assert(1 == weight(o));
  assert(0 == value(o));
  assert(0 == depth(0));

  /* second insert */
  assert(insert(o, 1));
  assert(2 == weight(o));
  assert(8 == depth(o));

  /* duplicate insert should change nothing */
  assert(insert(o, 1));
  assert(2 == weight(o));
  assert(8 == depth(o));

  destroy(o);

  return EXIT_SUCCESS;

  std::cout << std::hex << std::showbase << std::endl
            << "octree::node * o == " << o << std::endl
            << "octree::value( o ) == " << octree::value(o) << std::dec
            << std::noshowbase << std::endl
            << "octree::weight( o ) == " << octree::weight(o) << std::endl;

  o = octree::insert(o, 0x42);

  std::cout << std::hex << std::showbase << std::endl
            << "octree::node * o == " << o << std::endl
            << "octree::value( o ) == " << octree::value(o) << std::dec
            << std::noshowbase << std::endl
            << "octree::weight( o ) == " << octree::weight(o) << std::endl;

  /* delete o ; */
  octree::destroy(o);
  o = nullptr;

  std::cout << std::hex << std::showbase << std::endl
            << "octree::node * o == " << o << std::endl
            << "octree::value( o ) == " << octree::value(o) << std::endl;

  return EXIT_SUCCESS;
}
