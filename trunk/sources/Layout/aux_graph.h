/**
 * @file: aux_graph.h - Layout Graph class - Graph for performing layouts, consists of dummy nodes.
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 * AuxGraph, AuxNode and AuxEdge classes represent auxiliary graph used for layout purposes
 */
#ifndef AUX_GRAPH_H
#define AUX_GRAPH_H

/**
 * Rank type and its undefined constant
 */
typedef unsigned int Rank;
const Rank RANK_UNDEF = (Rank) (-1);

/**
 * Types of a node
 *
 * @ingroup Layout
 */
enum AuxNodeType
{
    /* Simple aux node that represents one node of processed graph */
    AUX_NODE_SIMPLE,
    /* Aux node that represents a control point of edge in processed graph */
    AUX_EDGE_CONTROL,
    /* Number of aux node types */
    AUX_NODE_TYPES_NUM
};

/**
 * Represents nodes and edge controls in Layout
 * 
 * @ingroup Layout
 */
class AuxNode: public NodeT< AuxGraph, AuxNode, AuxEdge>
{
    double priv_x;
    double priv_y;
    double priv_height;
    double priv_width;
    qreal barycenter;
    int priv_priority;
    Level * priv_level;
    int priv_order;
    Rank priv_rank;
    AuxNodeType node_type;

public:
    /** Get Height */
    virtual double height() const
    {
        return priv_height;
    }
    /** Get Width */
    virtual double width() const
    {
        return priv_width;
    }
    inline double modelX() const
    {
        return priv_x;
    }
    inline double modelY() const
    {
        return priv_y;
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
    inline void setX( double x)
    {
        priv_x = x;
    }
    inline void setY( double y)
    {
        priv_y = y;
    }
    inline qreal bc() const
    {
        return barycenter;
    }
    inline void setBc( qreal center)
    {
        barycenter = center;
    }
    /** Set Height */
    inline void setHeight( double h) 
    {
        priv_height = h;
    }
    /** Set Width */
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
    inline void setType( AuxNodeType t)
    {
        node_type = t;
    }
    inline AuxNodeType type() const
    {
        return node_type;
    }
    inline bool isSimple() const
    {
        return node_type == AUX_NODE_SIMPLE;
    }
    inline bool isEdgeControl() const
    {
        return node_type == AUX_EDGE_CONTROL;
    }
    inline void setTypeEdgeControl()
    {
        node_type = AUX_EDGE_CONTROL;
    }
    inline void setTypeSimple()
    {
        node_type = AUX_NODE_SIMPLE;
    }

    inline void debugPrint()
    {
        switch( node_type)
        {
            case AUX_NODE_SIMPLE:
                out("SIMPLE %llu;", id());
                break;
            case AUX_EDGE_CONTROL:
                out("EDGE CONTROL %llu;", id());
                break;
            default:         
                assert( 0);
                out("NO_TYPE %llu;", id());
                break;
        }
    }

    /**
     * Return value of spacing between previous and current node due to their types
     */
    inline qreal spacing( AuxNodeType prev_type) const
    {
        switch ( prev_type)
        {
            case AUX_NODE_SIMPLE:
                if ( node_type == AUX_NODE_SIMPLE)
                {
                    return NODE_NODE_MARGIN;
                } else
                {
                    return NODE_CONTROL_MARGIN;
                }
            case AUX_EDGE_CONTROL:
                if ( node_type == AUX_NODE_SIMPLE)
                {
                    return NODE_CONTROL_MARGIN;
                } else
                {
                    return CONTROL_CONTROL_MARGIN;
                }
            case AUX_NODE_TYPES_NUM:
                return 0;
        }
        return NODE_NODE_MARGIN;
    }

protected:
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
        node_type( AUX_NODE_SIMPLE)
    {
    }
    friend class GraphT< AuxGraph, AuxNode, AuxEdge>;
    friend class AuxGraph;
};
/**
 * Edge of model graph used in Layout
 *
 * @ingroup Layout
 */
class AuxEdge: public EdgeT< AuxGraph, AuxNode, AuxEdge>
{
    bool priv_fixed;

    /** Whether an edge should be inverted */
    bool is_back;
   
protected:
    /** Constructors are made private, only nodes and graph can create edges */
    AuxEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ):
        EdgeT< AuxGraph, AuxNode, AuxEdge>( graph_p, _id, _pred, _succ),
        priv_fixed( true), is_back( false) {};
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
    inline bool isBack() const
    {
        return is_back;
    }
    inline void setBack( bool back = true)
    {
        is_back = back;
    }
    inline bool isInverted() const
    {
        return isBack();
    }
};

/**
 * Graph with nodes of two types: simple nodes and edge controls
 *
 * @ingroup Layout
 */
class AuxGraph: public GraphT< AuxGraph, AuxNode, AuxEdge>
{
    /** Array of node lists for ranks */
    QVector< Level*> levels;

    /** Order numeration */
    Numeration order;
    
    /** Ranking numeration */
    Numeration ranking;

    /** Maximum used for ranking */
    GraphNum max_rank;

public:
    
    /** Iheritance and contructors */
    AuxGraph();
    ~AuxGraph();
    void initLevels( Rank max_level);
    void deleteLevels();
    
    void orderNodesByDFS();
    void reduceCrossings();
    void arrangeHorizontally();
    
    virtual void debugPrint()
    {
        out( "AuxGraph debug print");
        GraphT< AuxGraph, AuxNode, AuxEdge>::debugPrint();
    }

    virtual void * CreateNode( AuxGraph *graph_p, int _id)
    {
        return new AuxNode( graph_p, _id);
    }
    virtual void * CreateEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ)
    {
        return new AuxEdge( graph_p, _id, _pred, _succ);
    }
    inline Numeration ranks() const
    {
        return ranking;
    }
    inline GraphNum maxRank() const
    {
        return max_rank;
    }

    /** Perform layout */
    void doLayout();
    
    /** Assign ranks to nodes in respect to maximum length of path from top */
    Numeration rankNodes();

    /** Assign edge types, mark edges that should be inverted */
    void classifyEdges();
};

/**
 * Representation of rank level - group of nodes that should have same/close vertical position
 */
class Level
{
    Rank level_rank;
    QList< AuxNode*> node_list;
public:
    Level(): level_rank( 0), node_list(){};
    Level( Rank r): level_rank( r), node_list(){};
    
    void arrangeNodes( GraphDir dir, bool commit_placement, bool first_pass);
    void sortNodesByOrder();

    inline Rank rank() const
    {
        return level_rank;
    }
    inline void rank( Rank r)
    {
        level_rank = r;
    }
    inline QList< AuxNode*> nodes() const
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
