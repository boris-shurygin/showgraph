#include "graph_impl.h"

Graph::Graph()
{
    node_last_id = 0;
    edge_last_id = 0;
}

Node *
Graph::NewNode()
{
    NodeListIt it;
    nodes.push_back( NULL);
    it = nodes.end();
    it--;
    Node *node_p = new Node( this, node_last_id++, it);
    *it = node_p;
    return node_p;
}

Edge *
Graph::NewEdge( Node * pred, Node * succ)
{
    EdgeListIt it;
    Edge *edge_p = new Edge( this, edge_last_id++, pred, succ);
    edges.push_back( edge_p);
    it = edges.end();
    it--;
    edge_p->SetGraphIt( it);
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