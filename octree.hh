/* octree/octree.hh */

namespace octree
{
    struct node_t
    {
        node_t*parent_;
        node_t*children_[8];
        float value_;
    };

    node_t* create( void );
    void destroy( node_t* );
    node_t* insert( node_t*, int, int, int, float );
    node_t* remove( node_t*, int, int, int );
}
