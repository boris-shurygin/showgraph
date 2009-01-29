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

/**
 * Add an edge to this node in specified direction
 */
template <class Graph, class Node, class Edge>
void
NodeT<Graph, Node, Edge>::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    GraphAssert( IsNotNullP( edge));
    EdgeListIt it;
    edges[ dir].push_back( edge);
    it = edges[ dir].end();
    it--;
    edge->SetListIt( RevDir( dir), it);
}

/**
 * Delete edge pointed by iterator in specidied direction
 */
template <class Graph, class Node, class Edge>
void
NodeT<Graph, Node, Edge>::DeleteEdgeInDir( GraphDir dir, EdgeListIt it)
{
    edges[ dir].erase( it);
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