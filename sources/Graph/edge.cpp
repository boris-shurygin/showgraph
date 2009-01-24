#include "graph_impl.h"

/**
 * Edge destructor.
 * Delete edge from graph's list of edges
 */
Edge::~Edge()
{
    graph->DeleteEdge( graph_it);
}

void Edge::DebugPrint()
{
    out("%u->%u;", GetPred()->GetId(), GetSucc()->GetId());
}

void Edge::DetachFromNode( GraphDir dir)
{
    Node *n = GetNode( dir);
    n->DeleteEdgeInDir( RevDir( dir), n_it[ dir]);
}