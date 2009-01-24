#include "graph_impl.h"

Graph::Graph()
{
    node_last_id = 0;
    edge_last_id = 0;
}

Node *
Graph::NewNode()
{
    Node *node_p = new Node( this, node_last_id++);
    nodes.push_back( node_p);
    return node_p;
}

Edge *
Graph::NewEdge( Node * pred, Node * succ)
{
    Edge *edge_p = new Edge( this, edge_last_id++, pred, succ);
    edges.push_back( edge_p);
    return edge_p;
}

void 
Graph::DebugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = GetFirstNode(); !EndOfNodes(); n = GetNextNode())
    {
        n->DebugPrint();
    }
    /** Print edges */
    for (  e = GetFirstEdge(); !EndOfEdges(); e = GetNextEdge())
    {
        e->DebugPrint();
    }
    out( "}");
}