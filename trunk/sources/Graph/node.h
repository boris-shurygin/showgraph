#ifndef NODE_H
#define NODE_H
#include "graph_iface.h"

class Node
{
    int id;
    Graph * graph;
    NodeList::iterator my_it;

    //Lists of edges
    EdgeList edges[ GRAPH_DIRS_NUM];
    EdgeListIt e_it[ GRAPH_DIRS_NUM];
    
    /** We can't create nodes separately, do it through NewNode method of graph */
    Node( Graph *graph_p, int _id, NodeList::iterator it):id(_id), graph(graph_p), my_it( it){};
    friend class Graph;

public:
    ~Node();
    inline int GetId() const
    {
        return id;
    }
    inline Graph * GetGraph() const
    {
        return graph;
    }
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
    void AddEdgeInDir( Edge *edge, GraphDir dir);
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
        return *e_it[ dir ];
    }
    inline Edge* GetNextEdgeInDir( GraphDir dir)
    {
        e_it[ dir]++;
        return (e_it[ dir] != edges[ dir].end())? *e_it[ dir] : NULL;
    }
    inline bool EndOfEdgesInDir( GraphDir dir)
    {
        return e_it [ dir] == edges [ dir ].end();
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
    void DeleteEdgeInDir( GraphDir dir, EdgeListIt it);
    inline void DeletePred( EdgeListIt it)
    {
        DeleteEdgeInDir( GRAPH_DIR_UP, it);
    }
    inline void DeleteSucc( EdgeListIt it)
    {
        DeleteEdgeInDir( GRAPH_DIR_DOWN, it);
    }
    void DebugPrint();
};

#endif
