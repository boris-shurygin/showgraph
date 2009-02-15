/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

class EdgeW: public QGraphicsItem, EdgeT< GraphW, NodeW, EdgeW>
{
    /** Constructors are made private, only nodes and graph can create edges */
    EdgeW( GraphW *graph_p, int _id, NodeW *_pred, NodeW* _succ):
        EdgeT< GraphW, NodeW, EdgeW>( graph_p, _id, _pred, _succ){};
        
    friend class GraphT< GraphW, NodeW, EdgeW>;
    friend class NodeT< GraphW, NodeW, EdgeW>;
    friend class GraphW;
    friend class NodeW;
public:
    
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
#endif