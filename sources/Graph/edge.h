#ifndef EDGE_H
#define EDGE_H
#include "graph_iface.h"

class Edge
{
    /** Graph part */
    int id; //Unique ID
    Graph * graph; //Graph
    EdgeListIt graph_it; //Position in Graph's list of edges

    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Incidient nodes
    EdgeListIt n_it[ GRAPH_DIRS_NUM];//Position in each node's list
    
    /** Constructors are made private, only nodes and graph can create edges */
    Edge( Graph *graph_p, int _id, Node *_pred, Node* _succ): id(_id), graph(graph_p)
    {
        SetPred( _pred);
        SetSucc( _succ);
    }


    /** Pivate routines dealing with iterators */
    void SetGraphIt( EdgeListIt g_it)
    {
        graph_it = g_it;
    }

    EdgeListIt GetGraphIt()
    {
        return graph_it;
    }

    void SetListIt( GraphDir dir, EdgeListIt it)
    {
        n_it[ dir] = it;
    }

    EdgeListIt GetNodeIt( GraphDir dir)
    {
        return n_it[ dir];
    }

    
    /** Graph and Node have access to Edge's members */
    friend class Node;
    friend class Graph;
public:
    ~Edge();
    void SetNode( Node *n, GraphDir dir)
    {
        nodes[ dir] = n;
        if ( n != NULL)
        {
            n->AddEdgeInDir( this, 
                ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
        }
    }
    inline void SetPred( Node *n)
    {
        SetNode( n, GRAPH_DIR_UP);
    }
    inline void SetSucc( Node *n)
    {
        SetNode( n, GRAPH_DIR_DOWN);
    }
    inline Node *GetNode( GraphDir dir) const 
    {
        return nodes[ dir];
    }
    inline Node *GetPred() const 
    {
        return GetNode( GRAPH_DIR_UP);
    }
    inline Node *GetSucc() const 
    {
        return GetNode( GRAPH_DIR_DOWN);
    }
    void DetachFromNode( GraphDir dir);
    void DebugPrint();
};

#endif
