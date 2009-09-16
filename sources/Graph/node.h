/**
 * File: node.h - Node class definition, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_H
#define NODE_H

/**
 * Node representation class. 
 */
template <class Graph, class Node, class Edge> class NodeT: public Marked, public Numbered
{
public:
    typedef ListItem< Node> NodeListIt;
    typedef ListItem< Edge> EdgeListIt;
private:
    /** Representation in document */
    QDomElement element;

    /** Connection with inclusive graph */
    GraphUid uid; // Unique id
    Graph * graph_p;// Pointer to graph
    NodeListIt graph_it;//Item of graph's list
protected:    
    //Lists of edges and iterators for them
    Edge *first_edge[ GRAPH_DIRS_NUM];

    NodeListIt* GetGraphIt()
    {
        return &graph_it;
    }
    /** We can't create nodes separately, do it through newNode method of graph */
    NodeT( Graph *_graph_p, GraphUid _id):uid(_id), graph_p( _graph_p), graph_it(), element()
    {
        first_edge[ GRAPH_DIR_UP] = NULL;
        first_edge[ GRAPH_DIR_DOWN] = NULL;
        graph_it.setData( ( Node*)this);
    }
    inline void detachFromGraph()
    {
        graph_it.Detach();
    }
    friend class Graph;

public:
    /**
     * Destructor
     */
    virtual ~NodeT();
    
    inline QDomElement elem() const
    {
        return element;
    }
    
    inline void setElement( QDomElement elem)
    {
        element = elem;
    }

    /**
     * Get node's unique ID
     */
    inline GraphUid id() const
    {
        return uid;
    }

    /**
     * Get node's corresponding graph
     */
    inline Graph * graph() const
    {
        return graph_p;
    }

    inline Node* nextNode()
    {
        return ( graph_it.next() != NULL )? graph_it.next()->data() : NULL;
    }

    /**
     * Add edge to node in specified direction
     */
    void AddEdgeInDir( Edge *edge, GraphDir dir);

    /**
     * Add predecessor edge
     */
    inline void AddPred( Edge *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }

    /**
     * Add successor edge
     */
    inline void AddSucc( Edge *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    /**
     *  Iteration through edges routines.
     *
     *  Set iterator to beginning of edge list and return first edge
     */
    inline Edge* firstEdgeInDir( GraphDir dir)
    {
        return first_edge[ dir];
    }
    /** 
     * Corresponding iterators for successors/predeccessors.
     * NOTE: See firstEdgeInDir and other ...InDir routines for details
     */
    inline Edge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline Edge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    
    /**
     * Deletion of edge in specified direction
     */
    void deleteEdgeInDir( GraphDir dir, Edge* edge);
    
    /**
     * delete predecessor edge
     */
    inline void deletePred( Edge* edge)
    {
        deleteEdgeInDir( GRAPH_DIR_UP, edge);
    }
    
    /**
     * delete successor edge
     */
    inline void deleteSucc( Edge* edge)
    {
        deleteEdgeInDir( GRAPH_DIR_DOWN, edge);
    }

    /**
     * Print node in DOT format to stdout
     */
    virtual void debugPrint();

    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);
};

#include "node.cpp"

#endif
