/**
 * @file: edge.h
 * Implementation of Edge class inline routines
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_INLINE_H
#define EDGE_INLINE_H

/**
 * Connect edge to a node in specified direction.
 * Note that node treats this edge in opposite direction. I.e. an edge that has node in
 * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
 */
inline void 
Edge::setNode( Node *n, GraphDir dir)
{
    assert( isNotNullP( n));
    nodes[ dir] = n;
    if ( n != NULL)
    {
        n->AddEdgeInDir( (Edge *)this, 
            ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
    }
}

/**
 * Insert a node on this edge
 *
 * Creates a node on edge and a new edge from new node to former successor of original edge.
 * Original edge goes to new node. 
 * Return new node.
 */
inline Node *
Edge::insertNode()
{
    Node *tmp_succ = succ();
    Node *new_node = graph()->newNode();
    detachFromNode( GRAPH_DIR_DOWN);
    setSucc( new_node);
    graph()->newEdge( new_node, tmp_succ);
    return new_node;
}

#endif