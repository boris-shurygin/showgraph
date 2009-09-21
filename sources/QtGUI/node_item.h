/**
 * File: node_w.h - Node Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

class NodeItem: public AuxNode, public QGraphicsTextItem
{    
    /** Initialization */
    inline void SetInitFlags()
    {
        QString text = QString("Node %1").arg( id());
        setPlainText( text);
        setFlag( ItemIsMovable);
        //setFlag(ItemIsSelectable);
        setCacheMode( DeviceCoordinateCache);
        setZValue(1);
    }

    /** We can't create nodes separately, do it through newNode method of graph */
    NodeItem( GraphView *graph_p, int _id): AuxNode( ( AuxGraph *)graph_p, _id)
    {
        SetInitFlags();
    }

    /** Contructor of node with specified position */
    NodeItem( GraphView *graph_p, int _id, QPointF _pos):
        AuxNode( ( AuxGraph *)graph_p, _id)
    {
        SetInitFlags();
        setPos( _pos);
    }

    friend class GraphT< GraphView, NodeItem, EdgeItem>;
    friend class GraphView;
public:
    virtual ~NodeItem();

    enum {Type = TypeNode};

    int type() const
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
    
    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    /** Graph part */
    GraphView * graph() const;
    inline NodeItem* nextNode()
    {
        return static_cast< NodeItem*>( AuxNode::nextNode());
    }
    inline void AddEdgeInDir( EdgeItem *edge, GraphDir dir)
    {
        AuxNode::AddEdgeInDir( (AuxEdge *)edge, dir);
    }
    inline void AddPred( EdgeItem *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }
    inline void AddSucc( EdgeItem *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    inline EdgeItem* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< EdgeItem*>( AuxNode::first_edge[ dir]);
    }
    inline EdgeItem* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline EdgeItem* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    virtual inline double width() const
    {
        return boundingRect().width();
    }
    virtual inline double height() const
    {
        return boundingRect().height();
    }
};

#endif