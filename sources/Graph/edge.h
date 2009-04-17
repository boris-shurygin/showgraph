/**
 * File: edge.h - Edge class definition, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_H
#define EDGE_H

/**
 *  Edge class implements basic concept of graph edge.
 *  It has two Nodes as its end points. As edge is member of 3 lists it
 *  has 3 corresponding iterators pointing to it in theses lists
 */
template <class Graph, class Node, class Edge> class EdgeT: public Marked, public Numbered
{
public:
    typedef ListItem< Edge> EdgeListIt;
private:
    /** Representation in document */
    QDomElement element;

    /** Graph part */
    int uid; //Unique ID
    Graph * graph; //Graph
    EdgeListIt graph_it; //Position in Graph's list of edges

    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    EdgeListIt n_it[ GRAPH_DIRS_NUM];//Position in each node's list

protected:
    /** Graph and Node have access to Edge's members */
    friend class Graph;
    friend class Node;

    /** Constructors are made private, only nodes and graph can create edges */
    EdgeT( Graph *graph_p, int _id, Node *_pred, Node* _succ): uid(_id), graph(graph_p), graph_it()
    {
        graph_it.SetData( (Edge*) this);
        n_it[ GRAPH_DIR_UP] = EdgeListIt();
        n_it[ GRAPH_DIR_DOWN] = EdgeListIt();
        n_it[ GRAPH_DIR_UP].SetData( (Edge*)this);
        n_it[ GRAPH_DIR_DOWN].SetData( (Edge*)this);
        setPred( _pred);
        setSucc( _succ);
    }
    /** Pivate routines dealing with iterators */
    void SetGraphIt( EdgeListIt g_it)
    {
        graph_it = g_it;
    }
    /** 
     * Return iterator pointing to this edge in graph's edge list
     */
    EdgeListIt *GetGraphIt()
    {
        return &graph_it;
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
    EdgeListIt *GetNodeIt( GraphDir dir)
    {
        return &n_it[ dir];
    }

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    void DetachFromNode( GraphDir dir);
public:
    
    inline QDomElement elem() const
    {
        return element;
    }
    
    inline void setElement( QDomElement elem)
    {
        element = elem;
    }

    /**
     * Get edge's unique ID
     */
    inline int id() const
    {
        return uid;
    }

    /**
     * Get edge's corresponding graph
     */
    inline Graph * GetGraph() const
    {
        return graph;
    }

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
    ~EdgeT();

    /**
     * Connect edge to a node in specified direction.
     * Note that node treats this edge in opposite direction. I.e. an edge that has node in
     * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
     */
    void SetNode( Node *n, GraphDir dir)
    {
        assert( IsNotNullP( n));
        nodes[ dir] = n;
        if ( n != NULL)
        {
            n->AddEdgeInDir( (Edge *)this, 
                ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
        }
    }
    
    /**
     * Connect edge with given node as a predecessor
     */
    inline void setPred( Node *n)
    {
        SetNode( n, GRAPH_DIR_UP);
    }
    /**
     * Connect edge with given node as a successor
     */
    inline void setSucc( Node *n)
    {
        SetNode( n, GRAPH_DIR_DOWN);
    }

    /**
     * Get node in specified direction
     */
    inline Node *node( GraphDir dir) const 
    {
        return nodes[ dir];
    }
    /**
     * Get predecessor of edge
     */
    inline Node *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /**
     * Get successor of edge
     */
    inline Node *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }
    /**
     * Print edge in dot fomat to stdout
     */
    void DebugPrint();

    /** 
     * Update DOM element
     */
    void updateElement();

};

#include "edge.cpp"

#endif
