/**
 * File: node_w.h - Node Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

class NodeW: public QGraphicsItem, public NodeT< GraphW, NodeW, EdgeW>
{
    /** Initialization */
    inline void SetInitFlags()
    {
        setFlag(ItemIsMovable);
        setCacheMode(DeviceCoordinateCache);
        setZValue(1);
    }

    /** We can't create nodes separately, do it through NewNode method of graph */
    NodeW( GraphW *graph_p, int _id):
        NodeT< GraphW, NodeW, EdgeW>( graph_p, _id)
    {
        SetInitFlags();
    }

    /** Contructor of node with specified position */
    NodeW( GraphW *graph_p, int _id, QPointF _pos):
        NodeT< GraphW, NodeW, EdgeW>( graph_p, _id)
    {
        SetInitFlags();
        setPos( _pos);
    }

    friend class GraphT< GraphW, NodeW, EdgeW>;
    friend class GraphW;
public:
    ~NodeW();

    int type()
    {
        return TypeNode;
    }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif