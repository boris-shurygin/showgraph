/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

#define EdgeControlSize 5
#include "gui_impl.h"

class GEdge: public AuxEdge
{
    EdgeItem *item_p;

    /** Constructors are made private, only nodes and graph can create edges */
    GEdge( GraphView *graph_p, int _id, GNode *_pred, GNode* _succ);
    virtual ~GEdge();

    friend class GraphT< GraphView, GNode, GEdge>;
    friend class NodeT< GraphView, GNode, GEdge>;
    friend class GraphView;
    friend class Node;
public:
    /**
     * Return pointer to graph
     */
    GraphView * graph() const;

    /**
     * Return associated graphics item
     */
    inline EdgeItem *item() const
    {
        return item_p;
    }
    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    /** Graph part */
    inline void setNode( GNode *n, GraphDir dir)
    {
        AuxEdge::setNode( ( AuxNode *)n, dir);
    }
    inline void setPred( GNode *n)
    {
        setNode( n, GRAPH_DIR_UP);
    }
    inline void setSucc( GNode *n)
    {
        setNode( n, GRAPH_DIR_DOWN);
    }
    inline GNode *node( GraphDir dir) const;
    inline GNode *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    inline GNode *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }
    inline GEdge* nextEdge()
    {
        return static_cast< GEdge *>(AuxEdge::nextEdge());
    }
    inline GEdge* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< GEdge *>(AuxEdge::nextEdgeInDir( dir));
    }
    inline GEdge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline GEdge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
};

class EdgeItem: public QGraphicsItem
{
public:        
    typedef enum EdgeMode
    {
        ModeShow,
        ModeEdit
    } EdgeMode;

private:
    /** Graph connection */
    GEdge* edge_p;

    QPointF srcP;
    QPointF cp1;
    QPointF cp2;
    QPointF dstP;
    QPointF topLeft;
    QPointF btmRight;
    EdgeMode curr_mode;
public:
    
    enum {Type = TypeEdge};

    inline EdgeItem( GEdge *e_p);

    int type() const
    {
        return Type;
    }

    void adjust();
    
    inline GEdge* edge() const
    {
        return edge_p;    
    }
    
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
    
    QVariant itemChange( GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    inline GNode* pred() const
    {
        return edge()->pred();
    }
    inline GNode* succ() const
    {
        return edge()->succ();
    }
    inline void remove()
    {
        setVisible( false);
        removeFromIndex();
        scene()->removeItem( this);
        edge_p = NULL;
    }
};
#endif