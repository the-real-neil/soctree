/* octree/test/create_delete.cc */

#include "octree.hh"

/* #include <iostream> */
/* #include <cstdlib> */

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
    octree::node*o = octree::init();

    /* std::cout */
    /*     << "node_t*o == " */
    /*     << std::hex << std::showbase */
    /*     << reinterpret_cast< unsigned long >( o ) */
    /*     << std::endl */
    /*     ; */

    /* delete o ; */
    octree::destroy( o );

    return 0 ;
}
