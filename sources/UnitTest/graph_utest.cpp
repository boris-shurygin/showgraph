#include "utest_impl.h"
#include <vector>

using namespace std;

bool UTestGraph()
{
    Graph graph;

    /** Create some graph */
    vector<Node *> nodes;
    for ( int i =0; i<20; i++)
    {
        nodes.push_back( graph.NewNode());
        if ( i > 0)
        {
            graph.NewEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.NewEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    
    graph.NewEdge( nodes[ 8], nodes[ 4]);
    graph.DebugPrint();
    /** Nodes traversal */
    //Assert<Error>( 0);
    return true;
}