#ifndef GRAPH_H
#define GRAPH_H
#include "graph_iface.h"

class Graph
{
    /* List of nodes and its iterator */
    list<Node *> nodes;
    list<Node *>::iterator n_it;
    unsigned int node_last_id;

    /* List of edges and its iterator */
    list<Edge *> edges;
    list<Edge *>::iterator e_it;
    unsigned int edge_last_id;

public:
    Graph();
    Graph( const Graph& g);
    Node * NewNode();
    Edge * NewEdge( Node * pred, Node * succ);
    
    inline void DeleteNode( NodeListIt it)
    {
        nodes.erase( it);
    }
    inline size_t GetNodeCount()
    {
        return nodes.size();
    }
    inline size_t GetEdgeCount()
    {
        return edges.size();
    }
    inline Edge* GetFirstEdge()
    {
        e_it = edges.begin();
        return *e_it;
    }
    inline Edge* GetNextEdge()
    {
        e_it++;
        return (e_it != edges.end())? *e_it : NULL;
    }
    inline bool EndOfEdges()
    {
        return e_it == edges.end();
    }
    inline Node* GetFirstNode()
    {
        n_it = nodes.begin();
        return *n_it;
    }
    inline Node* GetNextNode()
    {
        n_it++;
        return ( n_it != nodes.end())? *n_it : NULL;
    }
    inline bool EndOfNodes()
    {
        return n_it == nodes.end();
    }
    void DebugPrint();
};
#endif
