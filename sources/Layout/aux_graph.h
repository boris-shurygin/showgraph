/**
 * File: aux_graph.h - Layout Graph class - Graph for performing layouts, consists of dummy nodes.
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef AUX_GRAPH_H
#define AUX_GRAPH_H

/**
 * Rank type and its undefined constant
 */
typedef unsigned int Rank;
const Rank RANK_UNDEF = (Rank) (-1);

/**
 * AuxGraph, AuxNode and AuxEdge classes represent auxiliary graph used for layout purposes
 */
class AuxNode: public NodeT< AuxGraph, AuxNode, AuxEdge>
{
    NodeItem* orig_node;
    double priv_x;
    double priv_y;
    double priv_height;
    double priv_width;
    int priv_priority;
    Level * priv_level;
    int priv_order;
    Rank priv_rank;

public:
    /** Get Height */
    inline double h() const
    {
        return priv_height;
    }
    /** Get Width */
    inline double w() const
    {
        return priv_width;
    }
    inline int priority() const
    {
        return priv_priority;
    }
    inline int rank() const
    {
        return priv_rank;
    }
    inline int order() const
    {
        return priv_order;
    }
    /** Get Height */
    inline void setHeight( double h) 
    {
        priv_height = h;
    }
    /** Get Width */
    inline void setWidth( double w) 
    {
        priv_width = w;
    }
    inline void setPriority( int p) 
    {
        priv_priority = p;
    }
    inline void setLevel( Level* l) 
    {
        priv_level = l;
    }
    inline void setOrder( int or) 
    {
        priv_order = or;
    }
    inline void setRank( Rank r)
    {
        priv_rank = r;
    }
    inline NodeItem* orig() const
    {
        return orig_node;
    }
    inline void setOrig( NodeItem *n)
    {
        orig_node = n;
    }
private:
    /** We can't create nodes separately, do it through newNode method of graph */
    AuxNode( AuxGraph *graph_p, int _id):
        NodeT< AuxGraph, AuxNode, AuxEdge>( graph_p, _id),
        priv_x(0),
        priv_y(0),
        priv_height(0),
        priv_width(0),
        priv_priority(-1),
        priv_level( NULL),
        priv_order(-1),
        orig_node( NULL)
    {
    }
    friend class GraphT< AuxGraph, AuxNode, AuxEdge>;
    friend class AuxGraph;
};

class AuxEdge: public EdgeT< AuxGraph, AuxNode, AuxEdge>
{
    bool priv_fixed;

    /** Constructors are made private, only nodes and graph can create edges */
    AuxEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ):
        EdgeT< AuxGraph, AuxNode, AuxEdge>( graph_p, _id, _pred, _succ),
        priv_fixed( true)
    {
        
    }
public:
    friend class GraphT< AuxGraph, AuxNode, AuxEdge>;
    friend class NodeT< AuxGraph, AuxNode, AuxEdge>;
    friend class AuxGraph;
    friend class AuxNode;

    inline bool isFixed() const
    {
        return priv_fixed;
    }
    inline void setFixed( bool fx)
    {
        priv_fixed = fx;
    }

};

/**
 * Testing-purpose graph
 */
class AuxGraph: public GraphT< AuxGraph, AuxNode, AuxEdge>
{
    /** Array of node lists for ranks */
    QVector< Level*> levels;

    /** Graph that is subject to layout */
    GraphView *main_graph;

public:
    
    /** Iheritance and contructors */
    AuxGraph();
    AuxGraph( GraphView *graph_p);
    ~AuxGraph();
    void initLevels( Rank max_level);
    void deleteLevels();
    
    void reduceCrossings();
    void arrangeHorizontally();

    void * CreateNode( AuxGraph *graph_p, int _id, NodeListIt it)
    {
        return new AuxNode( graph_p, _id);
    }
    void * CreateEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ)
    {
        return new AuxEdge( graph_p, _id, _pred, _succ);
    }
};

/**
 * Representation of rank level - group of nodes that should have same/close vertical position
 */
class Level
{
    Rank level_rank;
    QLinkedList< AuxNode*> node_list;
public:
    Level(): level_rank( 0), node_list(){};
    Level( Rank r): level_rank( r), node_list(){};

    inline Rank rank() const
    {
        return level_rank;
    }
    inline void rank( Rank r)
    {
        level_rank = r;
    }
    inline QLinkedList< AuxNode*> nodes() const
    {
        return node_list;
    }
    inline void add( AuxNode *node)
    {
        node_list.push_back( node);
        node->setLevel( this);
        node->setRank( level_rank);
    }
};

#endif /** AUX_GRAPH_H */
