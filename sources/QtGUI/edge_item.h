/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

#define EdgeControlSize 5
#include "gui_impl.h"

class EdgeItem: public AuxEdge, public QGraphicsItem
{

public:        
    typedef enum EdgeMode
    {
        ModeShow,
        ModeEdit
    } EdgeMode;

private:
    QPointF srcP;
    QPointF cp1;
    QPointF cp2;
    QPointF dstP;
    QPointF topLeft;
    QPointF btmRight;
    qreal arrowSize;
    EdgeMode curr_mode;
    
    /** Constructors are made private, only nodes and graph can create edges */
    EdgeItem( GraphView *graph_p, int _id, NodeItem *_pred, NodeItem* _succ):
        AuxEdge( (AuxGraph *)graph_p, _id, (AuxNode *)_pred, (AuxNode *)_succ), arrowSize(10)
    {
        curr_mode = ModeShow;
        //setFlag( ItemIsSelectable);
    };
        
    virtual ~EdgeItem()
    {
        removeFromIndex();
        scene()->removeItem( this);
    }
    friend class GraphT< GraphView, NodeItem, EdgeItem>;
    friend class NodeT< GraphView, NodeItem, EdgeItem>;
    friend class GraphView;
    friend class NodeItem;
public:
    
    enum {Type = TypeEdge};

    int type() const
    {
        return Type;
    }

    void adjust();
   
    inline EdgeMode mode() const
    {
        return curr_mode;
    }

    inline void setMode( EdgeMode m)
    {
        curr_mode = m;
    }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    /** Graph part */
    void setNode( NodeItem *n, GraphDir dir)
    {
        AuxEdge::setNode( ( AuxNode *)n, dir);
    }
    inline void setPred( NodeItem *n)
    {
        setNode( n, GRAPH_DIR_UP);
    }
    inline void setSucc( NodeItem *n)
    {
        setNode( n, GRAPH_DIR_DOWN);
    }
    inline NodeItem *node( GraphDir dir) const;
    inline NodeItem *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    inline NodeItem *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }
    inline EdgeItem* nextEdge()
    {
        return static_cast< EdgeItem *>(AuxEdge::nextEdge());
    }
    inline EdgeItem* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< EdgeItem *>(AuxEdge::nextEdgeInDir( dir));
    }
    inline EdgeItem* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline EdgeItem* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    }
    
};
#endif