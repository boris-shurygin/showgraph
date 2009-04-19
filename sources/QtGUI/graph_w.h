/**
 * File: graph_w.h - Graph Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_W_H
#define GRAPH_W_H

class GraphView: public QGraphicsView, public GraphT< GraphView, NodeItem, EdgeItem>
{
    QPoint src;
    QPoint dst;
    bool createEdge;
    NodeItem *tmpSrc;
public:
    /** Constructor */
    GraphView();
    ~GraphView();

    void drawBackground(QPainter *painter, const QRectF &rect);
    
    /** New node/edge overloads */
    NodeItem* newNode();
    NodeItem* newNode( QDomElement e);
    EdgeItem* newEdge( NodeItem* pred, NodeItem* succ);
    EdgeItem* newEdge( NodeItem* pred, NodeItem* succ, QDomElement e);
        
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawForeground(QPainter *painter, const QRectF &rect);
    inline bool IsCreateEdge() const
    {
        return createEdge;
    }
    inline void SetCreateEdge( bool val)
    {
        createEdge = val;
    }
    inline void SetTmpSrc( NodeItem* node)
    {
        tmpSrc = node;
    }
    inline NodeItem* GetTmpSrc()
    {
        return tmpSrc;
    }
};

#endif