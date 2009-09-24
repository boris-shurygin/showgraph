/**
 * File: node_w.h - Node Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H


class NodeItem: public QGraphicsTextItem
{    
    GNode *node_p;    
    
    /** Initialization */
    void SetInitFlags();
public:
    enum {Type = TypeNode};

    /** Constructor */
    inline NodeItem( GNode *n_p)
    {
        node_p = n_p;
        SetInitFlags();
    }

    inline GNode *node() const
    {
        return node_p;
    }
    inline int type() const
    {
        return Type;
    }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    
    inline void remove()
    {
        setVisible( false);
        removeFromIndex();
        scene()->removeItem( this);
        node_p = NULL;
    }
};

class GNode: public AuxNode
{
    NodeItem *item_p;
    
    /** We can't create nodes separately, do it through newNode method of graph */
    GNode( GraphView *graph_p, int _id);
    /** Contructor of node with specified position */
    GNode( GraphView *graph_p, int _id, QPointF _pos);

    friend class GraphT< GraphView, GNode, GEdge>;
    friend class GraphView;
    
public:
    virtual ~GNode();

    inline NodeItem* item() const
    {
        return item_p;
    }

    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    GraphView * graph() const;

    inline GNode* nextNode()
    {
        return static_cast< GNode*>( AuxNode::nextNode());
    }
    inline void AddEdgeInDir( GEdge *edge, GraphDir dir)
    {
        AuxNode::AddEdgeInDir( (AuxEdge *)edge, dir);
    }
    inline void AddPred( GEdge *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }
    inline void AddSucc( GEdge *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    inline GEdge* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< GEdge*>( AuxNode::first_edge[ dir]);
    }
    inline GEdge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline GEdge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    virtual inline double width() const
    {
        return item()->boundingRect().width();
    }
    virtual inline double height() const
    {
        return item()->boundingRect().height();
    }
};
#endif