/* octree/demo.cc */

#include "octree.hh"

#include <iostream>
#include <cstdlib>

struct foo
{
  foo* parent_ ;
  foo* children_[8] ;
};

int
main( int /* argc */, char**/* argv */ )
{
  foo*f = new foo ;

  delete f ;

  f = NULL ;
  
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

  /* octree::node*o = octree::init(); */

  /* std::cout */
  /*     << "node_t*o == " */
  /*     << std::hex << std::showbase */
  /*     << reinterpret_cast< unsigned long >( o ) */
  /*     << std::endl */
  /*     ; */

  /* octree::destroy( o ); */

  return EXIT_SUCCESS ;
}
