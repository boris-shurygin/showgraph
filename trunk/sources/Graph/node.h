#ifndef NODE_H
#define NODE_H
#include "graph_impl.h"

class Node
{
    int id;
    Graph * graph;
    
    //Lists of edges
    list<Edge *> edges[ GRAPH_DIRS_NUM];
    list<Edge *>::iterator e_it[ GRAPH_DIRS_NUM];

public:
    //Constructor
    Node( Graph *graph_p, int _id):id(_id), graph(graph_p){};
    
    inline int GetId() const
    {
        return id;
    }
    inline Graph * GetGraph() const
    {
        return graph;
    }
    inline list<Edge *>& GetEdgesInDir( GraphDir dir)
    {
        list<Edge *>& res = edges[ dir];
        return res;
    }
    inline list<Edge *>& GetPreds()
    {
        return GetEdgesInDir( GRAPH_DIR_UP);
    }
    inline list<Edge *>& GetSuccs()
    {
        return GetEdgesInDir( GRAPH_DIR_DOWN);
    }
    inline void AddEdgeInDir( Edge *edge, GraphDir dir)
    {
        edges[ dir].push_back( edge);
    }
    inline void AddPred( Edge *edge)
    {
        edges[ GRAPH_DIR_UP].push_back( edge);
    }
    inline void AddSucc( Edge *edge) 
    {
        edges[ GRAPH_DIR_DOWN].push_back( edge);
    }
    inline Edge* GetFirstEdgeInDir( GraphDir dir)
    {
        e_it[ dir ] = edges[ dir ].begin();
        return *e_it[ dir ]++;
    }
    inline Edge* GetNextEdgeInDir( GraphDir dir)
    {
        return *e_it[ dir]++;
    }
    inline bool EndOfEdgesInDir( GraphDir dir)
    {
        return e_it [ dir] != edges [ dir ].end();
    }
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
};

#endif