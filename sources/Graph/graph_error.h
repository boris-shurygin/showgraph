#ifndef GRAPH_ERROR_H
#define GRAPH_ERROR_H
#include "graph_impl.h"

/**
 * Error types for graph package
 */
enum GraphErrorType{
    
    //General error
    GRAPH_ERROR_GENERIC,
    //Wrong Direction
    GRAPH_ERROR_WRONG_DIR,

};

/**
 * Graph-specific errors description
 */
class GraphError
{
    GraphErrorType type;
    Graph *graph;
    Node *node1;
    Node *node2;
    Edge *edge;
    inline void InitFields()
    {
        graph = NULL;
        node1 = NULL;
        node2 = NULL;
        edge = NULL;
    }
public:
    GraphError()
    {
        type = GRAPH_ERROR_GENERIC;
        InitFields();
    }
    /**
     * Complete constructor. Parameters can be NULL except for the type;
     */
    GraphError( GraphErrorType tp, Graph *g, Node* n1, Node *n2, Edge* e):
           type( tp), graph( g), node1( n1), node2(n2), edge(e)
    {
          
    }
    void PrintMessage();
};

/**
 * Graph's assertion routines;
 */
inline void GraphAssert( bool asrt)
{
    Assert<GraphError> (asrt);
}
inline void GraphAssert( bool asrt, GraphError e)
{
    Assert<GraphError> (asrt, e);
}
#endif
