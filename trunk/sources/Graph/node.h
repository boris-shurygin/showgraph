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
template <class Graph, class Node, class Edge> class NodeT
{
public:
    typedef list<Edge*> EdgeList;
    typedef typename EdgeList::iterator EdgeListIt;
    typedef list<Node *> NodeList;
    typedef typename NodeList::iterator NodeListIt;
private:
    /** Connection with inclusive graph */
    int id; // Unique id
    Graph * graph;// Pointer to graph
    NodeListIt my_it; // Iterator pointing to node's position in graph's node list

    //Lists of edges and iterators for them
    EdgeList edges[ GRAPH_DIRS_NUM];
    EdgeListIt e_it[ GRAPH_DIRS_NUM];
    
protected:
    /** We can't create nodes separately, do it through NewNode method of graph */
    NodeT( Graph *graph_p, int _id, NodeListIt it):id(_id), graph(graph_p), my_it( it){};
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
    /******** routines are considered non-tested *************/
    inline EdgeList& GetEdgesInDir( GraphDir dir)
    {
        EdgeList& res = edges[ dir];
        return res;
    }
    inline EdgeList& GetPreds()
    {
        return GetEdgesInDir( GRAPH_DIR_UP);
    }
    inline EdgeList& GetSuccs()
    {
        return GetEdgesInDir( GRAPH_DIR_DOWN);
    }
    /**********************************************************/

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
        e_it[ dir ] = edges[ dir ].begin();
        return *e_it[ dir ];
    }
    /**
     * Advance iterator and return next edge in specified direction
     * NOTE: If end of list is reached we return NULL for first time and fail if called once again
     */
    inline Edge* GetNextEdgeInDir( GraphDir dir)
    {
        e_it[ dir]++;
        return (e_it[ dir] != edges[ dir].end())? *e_it[ dir] : NULL;
    }
    /**
     * Return true if iterator of list points to one-after-last element
     */
    inline bool EndOfEdgesInDir( GraphDir dir)
    {
        return e_it [ dir] == edges [ dir ].end();
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
    void DeleteEdgeInDir( GraphDir dir, EdgeListIt it);
    
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
