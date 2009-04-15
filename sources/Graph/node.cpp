/**
 * File: node.cpp - Node class implementation, part of Graph library, 
 * an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

/** 
 * Destructor. Corrects list of nodes in corresponding graph and deletes adjacent edges
 */
template <class Graph, class Node, class Edge>
NodeT<Graph, Node, Edge>::~NodeT()
{
    Edge *edge;
    
    /** Delete incidient edges */
    for ( edge = firstSucc(); !endOfSuccs();)
    {
        Edge* next = nextSucc();
        edge->DetachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        delete edge;
        edge = next;
    }
    for ( edge = firstPred(); !endOfPreds();)
    {
        Edge* next = nextPred();
        edge->DetachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        delete edge;
        edge = next;
    }
    /** Delete myself from graph */
    graph->DeleteNode( &my_it);
}

/**
 * Add an edge to this node in specified direction
 */
template <class Graph, class Node, class Edge>
void
NodeT<Graph, Node, Edge>::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    assert( IsNotNullP( edge));
    EdgeListIt *it = edge->GetNodeIt( RevDir( dir));
    it->Attach( edges[ dir]);
    edges[ dir] = it;
}

/**
 * Delete edge pointed by iterator in specidied direction
 */
template <class Graph, class Node, class Edge>
void
NodeT<Graph, Node, Edge>::DeleteEdgeInDir( GraphDir dir, EdgeListIt* it)
{
    assert( IsNotNullP( it));
    if( edges[ dir] == it)
    {
        edges[ dir] = it->next();
    }
    if( e_it[ dir] == it)
    {
        e_it[ dir] = it->next();
    }
    it->Detach();
}

/**
 * Print node in Dot format to stdout
 */
template <class Graph, class Node, class Edge>
void
NodeT<Graph, Node, Edge>::DebugPrint()
{
    out("%u;", GetId());
}