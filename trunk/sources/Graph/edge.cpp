/**
 * File: edge.cpp - Edge class implementation, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/**
 * Edge destructor.
 * Delete edge from graph's list of edges
 */
Edge::~Edge()
{
    graph->DeleteEdge( graph_it);
}

/**
 * Print edge in DOT format to stdout
 */
void Edge::DebugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
    GraphAssert( IsNotNullP( GetPred()));
    GraphAssert( IsNotNullP( GetSucc()));

    out("%u->%u;", GetPred()->GetId(), GetSucc()->GetId());
}

/**
 * Low level correction of node's edge list in corresponding direction
 */
void Edge::DetachFromNode( GraphDir dir)
{
    Node *n = GetNode( dir);
    n->DeleteEdgeInDir( RevDir( dir), n_it[ dir]);
}