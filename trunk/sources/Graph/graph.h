/**
 * File: graph.h - Graph class definition/implementation, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H
#include "graph_iface.h"

/**
 * Graph class decribes a graph.
 *  Like classical definition G = ( N, E) where N is set of nodes n and E is set of edges e = {n_i, n_j}
 */
class Graph
{
    /* List of nodes and its iterator */
    NodeList nodes;
    NodeListIt n_it;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphNum node_next_id;

    /* List of edges and its iterator */
    EdgeList edges;
    EdgeListIt e_it;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphNum edge_next_id;

public:
    /** Constructor */
    Graph();
    
    /** Create new node in graph */
    Node * NewNode();

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    Edge * NewEdge( Node * pred, Node * succ);
    
    /**
     * Remove node from node list of graph
     */
    inline void DeleteNode( NodeListIt it)
    {
        nodes.erase( it);
    }

    /**
     * Remove edge from edge list of graph
     */
    inline void DeleteEdge( EdgeListIt it)
    {
        edges.erase( it);
    }

    /**
     * Return node quantity
     */
    inline size_t GetNodeCount()
    {
        return nodes.size();
    }

    /**
     * Return edge quantity
     */
    inline size_t GetEdgeCount()
    {
        return edges.size();
    }
    /*** 
     * Iteration through edges implementation
     *
     * Initialize iterator with first edge and return this edge
     */
    inline Edge* GetFirstEdge() 
    {
        e_it = edges.begin();
        return *e_it;
    }
    /**
     * Advance iterator to next edge and return this edge. If end reached return NULL
     */
    inline Edge* GetNextEdge()
    {
        e_it++;
        return (e_it != edges.end())? *e_it : NULL;
    }
    /**
     * return true if end of edge list is reached
     */
    inline bool EndOfEdges()
    {
        return e_it == edges.end();
    }
    /*** 
     * Iteration through nodes implementation
     *
     * Initialize iterator with first node and return this node
     */
    inline Node* GetFirstNode()
    {
        n_it = nodes.begin();
        return *n_it;
    }
    /** 
     * Advance iterator to next node and return this node. If end reached return NULL
     */
    inline Node* GetNextNode()
    {
        n_it++;
        return ( n_it != nodes.end())? *n_it : NULL;
    }
    
    /**
     * return true if end of edge list is reached
     */
    inline bool EndOfNodes()
    {
        return n_it == nodes.end();
    }
    /**
     * Print graph to stdout in DOT format
     */
    void DebugPrint();
};

#endif
