#include "graph_impl.h"

Node::~Node()
{
    Edge *edge;
    
    /** Delete incidient edges */
    for ( edge = GetFirstSucc(); !EndOfSuccs(); edge = GetNextSucc())
    {
        edge->DetachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        delete edge;
    }
    for ( edge = GetFirstPred(); !EndOfPreds(); edge = GetNextPred())
    {
        edge->DetachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        delete edge;
    }
    /** Delete myself from graph */
    graph->DeleteNode( my_it);
}


void Node::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    GraphAssert( IsNotNullP( edge));
    EdgeListIt it;
    edges[ dir].push_back( edge);
    it = edges[ dir].end();
    it--;
    edge->SetListIt( RevDir( dir), it);
}
    
void Node::DeleteEdgeInDir( GraphDir dir, EdgeListIt it)
{
    edges[ dir].erase( it);
}
void Node::DebugPrint()
{
    out("%u;", GetId());
}