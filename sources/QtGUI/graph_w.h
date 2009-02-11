/**
 * File: graph_w.h - Graph Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_W_H
#define GRAPH_W_H

#include <QtGui/QGraphicsView>

class NodeW;
class EdgeW;

class GraphW: public QGraphicsView
{
    
public:
    /** Constructor */
    GraphW();

    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif