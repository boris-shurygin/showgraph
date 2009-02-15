/**
 * File: graph_w.h - Graph Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_W_H
#define GRAPH_W_H

class GraphW: public QGraphicsView, public GraphT< GraphW, NodeW, EdgeW>
{
    QPoint src;
    QPoint dst;
    bool createEdge;
public:
    /** Constructor */
    GraphW();
    ~GraphW();

    void drawBackground(QPainter *painter, const QRectF &rect);
    
    /** New node/edge overloads */
    NodeW* NewNode();
    EdgeW* NewEdge( NodeW* pred, NodeW* succ);
        
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawForeground(QPainter *painter, const QRectF &rect);
    
};

#endif