/**
 * File: edge.cpp - Edge class implementation, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

/**
 * Edge destructor.
 * Delete edge from graph's list of edges
 */
template <class Graph, class Node, class Edge>
EdgeT<Graph, Node, Edge>::~EdgeT()
{
    graph->DeleteEdge( &graph_it);
}

/**
 * Print edge in DOT format to stdout
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::DebugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
    assert( IsNotNullP( pred()));
    assert( IsNotNullP( succ()));

    out("%u->%u;", pred()->id(), succ()->id());
}

/**
 * Low level correction of node's edge list in corresponding direction
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::DetachFromNode( GraphDir dir)
{
    Node *n = node( dir);
    n->DeleteEdgeInDir( RevDir( dir), &n_it[ dir]);
}

/**
 * Update DOM tree element
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::updateElement()
{
    element.setAttribute( "source", pred()->id());
    element.setAttribute( "target", pred()->id());
}