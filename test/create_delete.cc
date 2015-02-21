/* octree/test/create_delete.cc */

#include "octree.hh"

#include <cstdlib>
#include <iostream>

int
main( int /* argc */, char**/* argv */ )
{
  /* std::cout */
  /*     << std::endl */
  /*     << __PRETTY_FUNCTION__ << " at " */
  /*     << __FILE__ << ":" << std::dec << __LINE__ */
  /*     << std::endl */
  /*     ; */

  /* for( int i = 0 ; i != argc ; ++i ) */
  /*     std::cout */
  /*         << "argv[i] == \"" << argv[i] << "\"" */
  /*         << std::endl */
  /*         ; */

  /* octree::node_t*o = new octree::node_t(); */
  octree::node*o = nullptr ;

  std::cout
    << std::hex << std::showbase
    << std::endl << "octree::node * o == " << o
    << std::endl << "octree::value( o ) == " << octree::value( o )
    << std::endl ;

  o = octree::init();

  std::cout
    << std::hex << std::showbase
    << std::endl << "octree::node * o == " << o
    << std::endl << "octree::value( o ) == " << octree::value( o )
    << std::endl ;
  /* std::cout */
  /*     << "node_t*o == " */
  /*     << std::hex << std::showbase */
  /*     << reinterpret_cast< unsigned long >( o ) */
  /*     << std::endl */
  /*     ; */

  /* delete o ; */
  octree::destroy( o );

  std::cout
    << std::hex << std::showbase
    << std::endl << "octree::node * o == " << o
    /* << std::endl << "octree::value( o ) == " << octree::value( o ) */
    << std::endl ;

  return EXIT_SUCCESS ;
}

