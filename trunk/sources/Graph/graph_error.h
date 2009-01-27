/**
 * File: graph_error.h - Definition of possible errors in Graph library, 
 * an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_ERROR_H
#define GRAPH_ERROR_H
#include "graph_impl.h"

/**
 * Error types for graph package
 */
enum GraphErrorType{
    
    //General error
    GRAPH_ERROR_GENERIC,

    //Maximum node number reached
    GRAPH_ERROR_NODE_NUM_OVERFLOW,
    
    //Maximum edge number reached
    GRAPH_ERROR_EDGE_NUM_OVERFLOW,
    
    //Wrong Direction
    GRAPH_ERROR_WRONG_DIR
};

/**
 * Graph-specific errors description class
 */
class GraphError
{
public:
    GraphErrorType type;
    Graph *graph;
    Node *node1;
    Node *node2;
    Edge *edge;

    /** Default constructor */
    GraphError()
    {
        type = GRAPH_ERROR_GENERIC;
        graph = NULL;
        node1 = NULL;
        node2 = NULL;
        edge = NULL;
    }
    /** Graph-level error constructor */
    GraphError( GraphErrorType tp, Graph *g): type( tp), graph( g)
    {
        node1 = NULL;
        node2 = NULL;
        edge = NULL;
    }
    
    /**
     * Complete constructor. Parameters can be NULL except for the type;
     */
    GraphError( GraphErrorType tp, Graph *g, Node* n1, Node *n2, Edge* e):
    type( tp), graph( g), node1( n1), node2(n2), edge(e){};
    
    /** Printing routine */
    void PrintMessage();
};

#endif
