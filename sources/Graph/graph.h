/**
 * File: graph.h - Graph class definition/implementation, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

/**
 * Graph class decribes a graph.
 *  Like classical definition G = ( N, E) where N is set of nodes n and E is set of edges e = {n_i, n_j}
 */
template <class Graph, class Node, class Edge > class GraphT: public MarkerManager, public NumManager, public QDomDocument
{
public:
    typedef ListItem< Node> NodeListIt;
    typedef ListItem< Edge> EdgeListIt;
private:
    /* List of nodes and its iterator */
    NodeListIt* nodes;
    NodeListIt* n_it;
    GraphNum node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphUid node_next_id;

    /* List of edges and its iterator */
    EdgeListIt* edges;
    EdgeListIt* e_it;
    GraphNum edge_num;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphUid edge_next_id;

    /**
     * Implementation of node/edge creation
     */
    Node * newNodeImpl( GraphUid id);
    Edge * newEdgeImpl( Node * pred, Node * succ);

public:
    /** Constructor */
    GraphT();
    
    /** Create new node in graph */
    Node * newNode();
    Node * newNode( QDomElement e);

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    Edge * newEdge( Node * pred, Node * succ);
    Edge * newEdge( Node * pred, Node * succ, QDomElement e);

    /**
     * Remove node from node list of graph
     */
    inline void DeleteNode( NodeListIt* it)
    {
        assert( IsNotNullP( it));
        if( nodes == it)
        {
           nodes = it->next();
        }
        if( n_it == it)
        {
            n_it = it->next();
        }
        it->Detach();

    }

    /**
     * Remove edge from edge list of graph
     */
    inline void DeleteEdge( EdgeListIt* it)
    {
        assert( IsNotNullP( it));
        if( edges == it)
        {
            edges = it->next();
        }
        if( e_it == it)
        {
            e_it = it->next();
        }
        it->Detach();
    }

    /**
     * Return node quantity
     */
    inline size_t GetNodeCount() const
    {
        return node_num;
    }

    /**
     * Return edge quantity
     */
    inline size_t GetEdgeCount() const
    {
        return edge_num;
    }
    /*** 
     * Iteration through edges implementation
     *
     * Initialize iterator with first edge and return this edge
     */
    inline Edge* firstEdge() 
    {
        e_it = edges;
        return (e_it != NULL)? e_it->GetData() : NULL;
    }
    /**
     * Advance iterator to next edge and return this edge. If end reached return NULL
     */
    inline Edge* nextEdge()
    {
        e_it = e_it->next();
        return (e_it != NULL)? e_it->GetData() : NULL;
    }
    /**
     * return true if end of edge list is reached
     */
    inline bool endOfEdges()
    {
        return e_it == NULL;
    }
    /*** 
     * Iteration through nodes implementation
     *
     * Initialize iterator with first node and return this node
     */
    inline Node* firstNode()
    {
        n_it = nodes;
        return ( n_it != NULL)? n_it->GetData() : NULL;
    }
    /** 
     * Advance iterator to next node and return this node. If end reached return NULL
     */
    inline Node* nextNode()
    {
        n_it = n_it->next();
        return ( n_it != NULL)? n_it->GetData() : NULL;
    }
    
    /**
     * return true if end of edge list is reached
     */
    inline bool endOfNodes()
    {
        return n_it == NULL;
    }
    /**
     * Print graph to stdout in DOT format
     */
    void DebugPrint();
    
    /** Node/Edge creation routines can be overloaded by derived class */
    void * CreateNode( Graph *graph_p, int _id);
    void * CreateEdge( Graph *graph_p, int _id, Node *_pred, Node* _succ);
    
    /**
     * Clear unused markers from marked objects
     */
    void clearMarkersInObjects();

    /**
     * Clear unused numerations from numbered objects
     */
    void clearNumerationsInObjects();

    /**
     * Save graph as an XML file
     */
    void writeToXML( QString filename);

    /**
     * Build graph from XML description
     */
    void readFromXML( QString filename);
};

#include "graph.cpp"

#endif
