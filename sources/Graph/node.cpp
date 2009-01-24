#include "graph_impl.h"

Node::~Node()
{
    /** Delete myself from graph */
    graph->DeleteNode( my_it);
}

void Node::DebugPrint()
{
    out("%u;", GetId());
}