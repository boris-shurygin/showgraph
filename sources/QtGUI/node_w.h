/**
 * File: node_w.h - Node Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

class NodeW: public QGraphicsItem, public NodeT< GraphW, NodeW, EdgeW>
{
    /** Widget data */
    
    /** We can't create nodes separately, do it through NewNode method of graph */
    NodeW( GraphW *graph_p, int _id):
        NodeT< GraphW, NodeW, EdgeW>( graph_p, _id){};

    /** Contructor of node with specified position */
    NodeW( GraphW *graph_p, int _id, QPointF _pos):
        NodeT< GraphW, NodeW, EdgeW>( graph_p, _id)
    {
        setPos( _pos);
    }

    friend class GraphT< GraphW, NodeW, EdgeW>;
    friend class GraphW;
public:
    ~NodeW();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif