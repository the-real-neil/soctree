/* octree/test/create_delete.cc */

#include "octree.hh"

#include <cassert>
#include <cstdlib>
#include <iostream>


int
main( int, char** )
{
  using namespace octree ;

  node*o = nullptr ;

  assert( nullptr == o );
  assert( 0 == weight(o) );
  assert( -1 == value(o) );

  o = init();

  assert( nullptr != o );
  assert( 0 == weight(o) );
  assert( -1 == value(o) );

  insert(o,0x42);

  assert( 1 == weight(o) );
  assert( 0x42 == value(o) );

  return EXIT_SUCCESS ;

  std::cout
    << std::hex << std::showbase
    << std::endl << "octree::node * o == " << o
    << std::endl << "octree::value( o ) == " << octree::value( o )
    << std::dec << std::noshowbase
    << std::endl << "octree::weight( o ) == " << octree::weight( o )
    << std::endl ;

  o = octree::insert(o,0x42);

  std::cout
    << std::hex << std::showbase
    << std::endl << "octree::node * o == " << o
    << std::endl << "octree::value( o ) == " << octree::value( o )
    << std::dec << std::noshowbase
    << std::endl << "octree::weight( o ) == " << octree::weight( o )
    << std::endl ;

  /* delete o ; */
  octree::destroy( o );
  o = nullptr ;

  std::cout
    << std::hex << std::showbase
    << std::endl << "octree::node * o == " << o
    << std::endl << "octree::value( o ) == " << octree::value( o )
    << std::endl ;

  return EXIT_SUCCESS ;
}

