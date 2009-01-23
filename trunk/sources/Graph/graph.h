#ifndef GRAPH_H
#define GRAPH_H
#include "graph_impl.h"

class Graph
{
    /* List of nodes and its iterator */
    list<Node *> nodes;
    list<Node *>::iterator n_it;
    int node_last_id;

    /* List of edges and its iterator */
    list<Edge *> edges;
    list<Edge *>::iterator e_it;
    int edge_last_id;

public:
    Graph();
    Graph( const Graph& g);
    Node * NewNode();
    Edge * NewEdge( Node * pred, Node * succ);
};
#endif