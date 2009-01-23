#ifndef EDGE_H
#define EDGE_H
#include "graph_impl.h"

class Edge
{
    //Graph part
    int id;
    Graph * graph;
    
    //Incidient nodes
    Node *nodes[ GRAPH_DIRS_NUM];
    
public:
    //Constructor
    Edge( Graph *graph_p, int _id):id(_id), graph(graph_p)
    {
        SetPred( NULL);
        SetSucc( NULL);
    }
    Edge( Graph *graph_p, int _id, Node *_pred, Node* _succ): id(_id), graph(graph_p)
    {
        SetPred( _pred);
        SetSucc( _succ);
    }
    void SetNode( Node *n, GraphDir dir)
    {
        nodes[ dir] = n;
        if ( n != NULL)
        {
            n->AddEdgeInDir( this, 
                ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
        }
    }
    void SetPred( Node *n)
    {
        SetNode( n, GRAPH_DIR_UP);
    }
    void SetSucc( Node *n)
    {
        SetNode( n, GRAPH_DIR_DOWN);
    }
    Node *GetNode( GraphDir dir)
    {
        return nodes[ dir];
    }
    Node *GetPred()
    {
        return GetNode( GRAPH_DIR_UP);
    }
    Node *GetSucc()
    {
        return GetNode( GRAPH_DIR_DOWN);
    }
};

#endif
