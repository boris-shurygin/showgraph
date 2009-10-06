/**
 * @file: graph.h 
 * Graph class definition/implementation, part of Graph library,
 * internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

/**
 * @defgroup Graph Graph representation
 *
 * Graph representation is implemented through 3 tightly connected classes Graph, Node and Edge.
 */

/**
 *  Graph representation  
 *
 *  @ingroup Graph
 *  Like classical definition G = ( N, E) where N is set of nodes n and E is set of edges e = {n_i, n_j}
 *  A graph has pointers to its first edge and node which allows iteration through them using nextEdge
 *  and nextNode methods of Node and Edge classes
 */
template <class Graph, class Node, class Edge > class GraphT: public MarkerManager, public NumManager, public QDomDocument
{
public:
    typedef ListItem< Node> NodeListIt;
    typedef ListItem< Edge> EdgeListIt;
private:
    /** First node */
    Node* first_node;
    /** Number of nodes */
    GraphNum node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphUid node_next_id;

    /* List of edges and its iterator */
    Edge* first_edge;
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
    
    /** Destructor */
    virtual ~GraphT();

    /** Create new node in graph */
    virtual Node * newNode();

    /** Create new node in graph and fills it with info in element */
    virtual Node * newNode( QDomElement e);

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    virtual Edge * newEdge( Node * pred, Node * succ);
    virtual Edge * newEdge( Node * pred, Node * succ, QDomElement e);

    /**
     * Remove node from node list of graph
     */
    inline void deleteNode( Node* node)
    {
        assert( isNotNullP( node));
        assert( node->graph() == this);

        if( first_node == node)
        {
           first_node = node->nextNode();
        }
        node->detachFromGraph();
    }

    /**
     * Remove edge from edge list of graph
     */
    inline void deleteEdge( Edge * edge)
    {
        assert( isNotNullP( edge));
        assert( edge->graph() == this);

        if( first_edge == edge)
        {
           first_edge = edge->nextEdge();
        }
        edge->detachFromGraph();
    }

    /**
     * Return node quantity
     */
    inline GraphNum nodeCount() const
    {
        return node_num;
    }

    /**
     * Return edge quantity
     */
    inline GraphNum edgeCount() const
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
        return first_edge;
    }
    
    /*** 
     * Iteration through nodes implementation
     *
     * Initialize iterator with first node and return this node
     */
    inline Node* firstNode()
    {
        return first_node;
    }
    /**
     * Print graph to stdout in DOT format
     */
    virtual void debugPrint();
    
    /** Node/Edge creation routines can be overloaded by derived class */
    virtual void * CreateNode( Graph *graph_p, int _id);
    virtual void * CreateEdge( Graph *graph_p, int _id, Node *_pred, Node* _succ);
    
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
