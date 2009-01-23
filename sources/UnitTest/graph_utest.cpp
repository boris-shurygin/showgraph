#include "utest_impl.h"

bool UTestGraph()
{
    Graph graph;
    Node * n = graph.NewNode();
    Node * m = graph.NewNode();
    graph.NewEdge( n, m);
    GraphAssert( 0, GraphError( GRAPH_ERROR_WRONG_DIR, 0, 0, 0, 0));
    return true;
}