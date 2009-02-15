/**
 * File: graph_w.h - Graph Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_W_H
#define GRAPH_W_H

class GraphW: public QGraphicsView, public GraphT< GraphW, NodeW, EdgeW>
{
    
public:
    /** Constructor */
    GraphW();
    ~GraphW();

    void drawBackground(QPainter *painter, const QRectF &rect);
    
    /** New node/edge overloads */
    NodeW* NewNode();
    EdgeW* NewEdge( NodeW* pred, NodeW* succ);
};

#endif