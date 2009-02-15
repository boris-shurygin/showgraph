/**
 * File: graph.cpp - Graph class implementation, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

/**
 * Constructor.
 */
template <class Graph, class Node, class Edge> 
GraphT< Graph, Node, Edge>::GraphT()
{
    node_next_id = 0;
    edge_next_id = 0;
    node_num = 0;
    edge_num = 0;
    nodes = NULL;
    edges = NULL;
    n_it = NULL;
    e_it = NULL;
}

/** Node/Edge creation routines can be overloaded by derived class */
template <class Graph, class Node, class Edge>
void * 
GraphT< Graph, Node, Edge>::CreateNode( Graph *graph_p, int _id)
{
    return new Node ( graph_p, _id);
}

template <class Graph, class Node, class Edge>
void * 
GraphT< Graph, Node, Edge>::CreateEdge( Graph *graph_p, int _id, Node *_pred, Node* _succ)
{
    return new Edge( graph_p, _id, _pred, _succ);
}

/**
 * Creation node in graph
 */
template <class Graph, class Node, class Edge>
Node * 
GraphT< Graph, Node, Edge>::NewNode()
{
    /**
     * Check that we have available node id 
     */
    GraphAssert( edge_next_id < GRAPH_MAX_NODE_NUM);
    NodeListIt* it;
    Node *node_p = ( Node *) CreateNode( (Graph *)this, node_next_id++);
    it = node_p->GetGraphIt();
    it->Attach( nodes);
    nodes = it;
    node_num++;
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
template <class Graph, class Node, class Edge>
Edge * 
GraphT< Graph, Node, Edge>::NewEdge( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    GraphAssert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = ( Edge *) CreateEdge( (Graph *)this, edge_next_id++, pred, succ);
    EdgeListIt* it = edge_p->GetGraphIt();
    it->Attach( edges);
    edges = it;
    edge_num++;
    return edge_p;
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
template <class Graph, class Node, class Edge>
void 
GraphT< Graph, Node, Edge>::DebugPrint()
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
