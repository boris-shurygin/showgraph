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
template <class Graph, class Node, class Edge> class NodeT: public Marked
{
public:
    typedef ListItem< Node> NodeListIt;
    typedef ListItem< Edge> EdgeListIt;
private:
    /** Connection with inclusive graph */
    int id; // Unique id
    Graph * graph;// Pointer to graph
    NodeListIt my_it;//Item of graph's list
    
    //Lists of edges and iterators for them
    EdgeListIt *edges[ GRAPH_DIRS_NUM];
    EdgeListIt *e_it[ GRAPH_DIRS_NUM];

protected:
    NodeListIt* GetGraphIt()
    {
        return &my_it;
    }
    /** We can't create nodes separately, do it through NewNode method of graph */
    NodeT( Graph *graph_p, int _id):id(_id), graph(graph_p), my_it()
    {
        edges[ GRAPH_DIR_UP] = NULL;
        edges[ GRAPH_DIR_DOWN] = NULL;
        e_it[ GRAPH_DIR_UP] = NULL;
        e_it[ GRAPH_DIR_DOWN] = NULL;
        my_it.SetData( ( Node*)this);
    }
    friend class Graph;

public:
    /**
     * Destructor
     */
    ~NodeT();
    
    /**
     * Get node's unique ID
     */
    inline int GetId() const
    {
        return id;
    }

    /**
     * Get node's corresponding graph
     */
    inline Graph * GetGraph() const
    {
        return graph;
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
        edges[ GRAPH_DIR_UP].push_back( edge);
    }

    /**
     * Add successor edge
     */
    inline void AddSucc( Edge *edge) 
    {
        edges[ GRAPH_DIR_DOWN].push_back( edge);
    }
    /**
     *  Iteration through edges routines.
     *
     *  Set iterator to beginning of edge list and return first edge
     */
    inline Edge* GetFirstEdgeInDir( GraphDir dir)
    {
        e_it[ dir ] = edges[ dir ];
        
        if ( e_it[ dir] == NULL)
        {
            return NULL;
        }
        return e_it[ dir ]->GetData();
    }
    /**
     * Advance iterator and return next edge in specified direction
     * NOTE: If end of list is reached we return NULL for first time and fail if called once again
     */
    inline Edge* GetNextEdgeInDir( GraphDir dir)
    {
        e_it[ dir] = e_it[ dir]->GetNext();
        return (e_it[ dir] != NULL )? e_it[ dir]->GetData() : NULL;
    }
    /**
     * Return true if iterator of list points to one-after-last element
     */
    inline bool EndOfEdgesInDir( GraphDir dir)
    {
        return e_it [ dir] == NULL;
    }

    /** 
     * Corresponding iterators for successors/predeccessors.
     * NOTE: See GetFirstEdgeInDir and other ...InDir routines for details
     */
    inline Edge* GetFirstSucc()
    {
        return GetFirstEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline Edge* GetNextSucc()
    {
        return GetNextEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline bool EndOfSuccs()
    {
        return EndOfEdgesInDir( GRAPH_DIR_DOWN);
    }
    inline Edge* GetFirstPred()
    {
        return GetFirstEdgeInDir( GRAPH_DIR_UP);
    }
    inline Edge* GetNextPred()
    {
        return GetNextEdgeInDir( GRAPH_DIR_UP);
    }
    inline bool EndOfPreds()
    {
        return EndOfEdgesInDir( GRAPH_DIR_UP);
    }

    /**
     * Deletion of edge in specified direction
     */
    void DeleteEdgeInDir( GraphDir dir, EdgeListIt* it);
    
    /**
     * Delete predecessor edge
     */
    inline void DeletePred( EdgeListIt it)
    {
        DeleteEdgeInDir( GRAPH_DIR_UP, it);
    }
    
    /**
     * Delete successor edge
     */
    inline void DeleteSucc( EdgeListIt it)
    {
        DeleteEdgeInDir( GRAPH_DIR_DOWN, it);
    }

    /**
     * Print node in DOT format to stdout
     */
    void DebugPrint();

};

#include "node.cpp"

#endif
