/**
 * File: edge.h - Edge class definition, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_H
#define EDGE_H
#include "graph_iface.h"

/**
 *  Edge class implements basic concept of graph edge.
 *  It has two Nodes as its end points. As edge is member of 3 lists it
 *  has 3 corresponding iterators pointing to it in theses lists
 */
class Edge
{
    /** Graph part */
    int id; //Unique ID
    Graph * graph; //Graph
    EdgeListIt graph_it; //Position in Graph's list of edges

    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    EdgeListIt n_it[ GRAPH_DIRS_NUM];//Position in each node's list
    
    /** Constructors are made private, only nodes and graph can create edges */
    Edge( Graph *graph_p, int _id, Node *_pred, Node* _succ): id(_id), graph(graph_p)
    {
        SetPred( _pred);
        SetSucc( _succ);
    }

    /** Pivate routines dealing with iterators */
    void SetGraphIt( EdgeListIt g_it)
    {
        graph_it = g_it;
    }
    /** 
     * Return iterator pointing to this edge in graph's edge list
     */
    EdgeListIt GetGraphIt()
    {
        return graph_it;
    }

    /** 
     * Set iterator pointing to this edge in graph's edge list
     */
    void SetListIt( GraphDir dir, EdgeListIt it)
    {
        n_it[ dir] = it;
    }

    /** 
     * Return iterator pointing to this edge in node's edge
     * list in corresponding direction
     */
    EdgeListIt GetNodeIt( GraphDir dir)
    {
        return n_it[ dir];
    }

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    void DetachFromNode( GraphDir dir);

    /** Graph and Node have access to Edge's members */
    friend class Node;
    friend class Graph;
public:
    /** 
     *  Destructor. Delete edge from list in graph.
     *  Deletion from node lists MUST be performed manually.
     *  Example: 
     *      Graph graph;
     *      Edge * edge = graph.NewEdge();
     *  
     *      //Typical deletion of edge is done by consequent calls of
     *      edge->DetachFromNode( GRAPH_DIR_UP);
     *      edge->DetachFromNode( GRAPH_DIR_DOWN);
     *      delete edge;
     */
    ~Edge();

    /**
     * Connect edge to a node in specified direction.
     * Note that node treats this edge in opposite direction. I.e. an edge that has node in
     * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
     */
    void SetNode( Node *n, GraphDir dir)
    {
        nodes[ dir] = n;
        if ( n != NULL)
        {
            n->AddEdgeInDir( this, 
                ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
        }
    }
    
    /**
     * Connect edge with given node as a predecessor
     */
    inline void SetPred( Node *n)
    {
        SetNode( n, GRAPH_DIR_UP);
    }
    /**
     * Connect edge with given node as a successor
     */
    inline void SetSucc( Node *n)
    {
        SetNode( n, GRAPH_DIR_DOWN);
    }

    /**
     * Get node in specified direction
     */
    inline Node *GetNode( GraphDir dir) const 
    {
        return nodes[ dir];
    }
    /**
     * Get predecessor of edge
     */
    inline Node *GetPred() const 
    {
        return GetNode( GRAPH_DIR_UP);
    }
    /**
     * Get successor of edge
     */
    inline Node *GetSucc() const 
    {
        return GetNode( GRAPH_DIR_DOWN);
    }
    /**
     * Print edge in dot fomat to stdout
     */
    void DebugPrint();
};

#endif
