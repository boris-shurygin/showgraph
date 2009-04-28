/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

#define EdgeControlSize 5
#include "gui_impl.h"

class EdgeSegment;

class EdgeControl: public QObject, public QGraphicsItem
{
    EdgeItem *edge; 
    EdgeSegment* predSeg;
    EdgeSegment* succSeg;
    bool isFixed;
    
public: 
        
    enum {Type = TypeEdgeControl};

    inline void setFixed( bool fixed = true)
    {
        isFixed = fixed;
        setFlag( ItemIsMovable, !fixed);
    }

    inline bool fixed()
    {
        return isFixed;
    }


    inline EdgeSegment* pred() const
    {
        return predSeg;
    }
    
    inline EdgeSegment* succ() const
    {
        return succSeg;
    }

    inline void setPred( EdgeSegment* p)
    {
        predSeg = p;
    }

    inline void setSucc( EdgeSegment* s)
    {
        succSeg = s;
    }

    int type() const
    {
        return Type;
    }

    EdgeControl( EdgeItem* e, QGraphicsScene* scene);
    ~EdgeControl();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void prepareRemove();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

class EdgeSegment: public QGraphicsItem
{
    EdgeItem* edge;
    QPointF srcP;
    QPointF cp1;
    QPointF cp2;
    QPointF dstP;
    EdgeControl* srcControl;
    EdgeControl* dstControl;
    QPointF topLeft;
    QPointF btmRight;

    enum SegmentType
    {
        LineSegment,
        CurveSegment
    };

public:
    
    inline EdgeControl* src() const
    {
        return srcControl;
    }

    inline EdgeControl* dst() const
    {
        return dstControl;
    }

    inline void setSrc( EdgeControl* s)
    {
        srcControl = s;
        if ( IsNotNullP( s))
        {
            s->setSucc( this);
        }  
    }

    inline void setDst( EdgeControl* d)
    {
        dstControl = d;
        if ( IsNotNullP( d))
        {
            d->setPred( this);
        } 
    }

    void adjust();

    EdgeControl *addControl( QPointF p);
    EdgeControl *addControl( EdgeControl* control);

    EdgeSegment( EdgeItem *e, EdgeControl *src, EdgeControl* dst, QGraphicsScene* scene);
    ~EdgeSegment();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

};

class EdgeItem: public QGraphicsItem, EdgeT< GraphView, NodeItem, EdgeItem>
{
public:        
    typedef enum EdgeMode
    {
        ModeShow,
        ModeEdit
    } EdgeMode;

private:
    qreal arrowSize;
    QPointF sourcePoint;
    QPointF destPoint;
    QPointF topLeft;
    QPointF bottomRight;
    QList< EdgeControl*> controls;
    QList< EdgeSegment*> segments;
    EdgeControl* srcControl;
    EdgeControl* dstControl;
    EdgeMode curr_mode;
    
    bool is_back;
    QList< AuxNode> aux_nodes;

    /** Constructors are made private, only nodes and graph can create edges */
    EdgeItem( GraphView *graph_p, int _id, NodeItem *_pred, NodeItem* _succ):
        EdgeT< GraphView, NodeItem, EdgeItem>( graph_p, _id, _pred, _succ), arrowSize(10)
        {
            srcControl = 0;
            dstControl = 0;
            curr_mode = ModeShow;
            //setFlag( ItemIsSelectable);
        };
        
    ~EdgeItem()
    {
        removeFromIndex();
        scene()->removeItem( this);
        foreach( EdgeControl* control, controls)
        {
            delete control;
        }
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

    inline bool isBack() const
    {
        return is_back;
    }

    inline setBack( bool back = true)
    {
        is_back = back;
    }

    void adjust();
    void addControl( EdgeControl* control);
    void removeControl( EdgeControl *control);
    void showControls();
    void hideControls();

    inline EdgeControl *srcCtrl() const
    {
        return srcControl;
    }
    inline EdgeControl *dstCtrl() const
    {
        return dstControl;
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
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void initControls();
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
};
#endif