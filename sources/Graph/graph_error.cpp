#include "graph_error.h"

void GraphError::PrintMessage()
{

}

void GraphAssert( bool asrt, GraphError e)
{
    out( "Graph node count is %d", e.graph->GetNodeCount());
    Assert<GraphError> (asrt, e);
}