/**
 * File: node_w.h - Node Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

class NodeItem: public QGraphicsTextItem, public NodeT< GraphW, NodeItem, EdgeItem>
{    
    QList< AuxNode> aux_nodes;

    /** Initialization */
    inline void SetInitFlags()
    {
        QString text = QString("Node %1").arg( GetId());
        setPlainText( text);
        setFlag( ItemIsMovable);
        //setFlag(ItemIsSelectable);
        setCacheMode( DeviceCoordinateCache);
        setZValue(1);
    }

    /** We can't create nodes separately, do it through NewNode method of graph */
    NodeItem( GraphW *graph_p, int _id):
        NodeT< GraphW, NodeItem, EdgeItem>( graph_p, _id)
    {
        SetInitFlags();
    }

    /** Contructor of node with specified position */
    NodeItem( GraphW *graph_p, int _id, QPointF _pos):
        NodeT< GraphW, NodeItem, EdgeItem>( graph_p, _id)
    {
        SetInitFlags();
        setPos( _pos);
    }

    friend class GraphT< GraphW, NodeItem, EdgeItem>;
    friend class GraphW;
public:
    ~NodeItem();

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
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif