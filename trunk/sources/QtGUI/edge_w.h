/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

#define EdgeControlSize 10


class EdgeControl: public QGraphicsItem
{
    EdgeW *edge; 
public: 
        
    
    enum {Type = TypeEdgeControl};

    int type() const
    {
        return Type;
    }

    EdgeControl( EdgeW* e): QGraphicsItem(), edge( e)
    {
        setFlag( ItemIsMovable);
        //setFlag(ItemIsSelectable);
        setCacheMode(DeviceCoordinateCache);
        setZValue(1);
    }
    QRectF boundingRect() const
    {
        qreal adjust = 2;
        return QRectF(-EdgeControlSize - adjust, -EdgeControlSize - adjust,
                      EdgeControlSize + adjust, EdgeControlSize + adjust);
    }

    QPainterPath shape() const
    {
        QPainterPath path;
        path.addEllipse(-EdgeControlSize, -EdgeControlSize, EdgeControlSize, EdgeControlSize);
        return path; 
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        if (option->state & QStyle::State_Sunken) {
            painter->setBrush( Qt::gray);
            painter->setPen( QPen( Qt::black, 0));
        } else {
            painter->setBrush( Qt::lightGray);
            painter->setPen( QPen(Qt::darkGray, 0));
        }
        painter->drawEllipse(-EdgeControlSize, -EdgeControlSize,
                              EdgeControlSize, EdgeControlSize);
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        update();
        QGraphicsItem::mousePressEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        update();
        QGraphicsItem::mouseReleaseEvent(event);
    }
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

class EdgeSegment: public QGraphicsPathItem
{
    EdgeW* edge;
    EdgeControl* srcControl;
    EdgeControl* dstControl;
public:

};

class EdgeW: public QGraphicsItem, EdgeT< GraphW, NodeW, EdgeW>
{
    qreal arrowSize;
    QPointF sourcePoint;
    QPointF destPoint;
    QPointF topLeft;
    QPointF bottomRight;
    QList< QPointF *> points;
    QList< EdgeControl*> controls;
    QList< EdgeSegment*> segments;

    typedef enum EdgeMode
    {
        ModeShow,
        ModeEdit
    } EdgeMode;

    EdgeMode mode;

    /** Constructors are made private, only nodes and graph can create edges */
    EdgeW( GraphW *graph_p, int _id, NodeW *_pred, NodeW* _succ):
        EdgeT< GraphW, NodeW, EdgeW>( graph_p, _id, _pred, _succ), arrowSize(10)
        {
            mode = ModeShow;
            setFlag( ItemIsSelectable);
            adjust();
        };
        
    ~EdgeW()
    {
        foreach( EdgeControl* control, controls)
        {
            delete control;
        }
    }
    friend class GraphT< GraphW, NodeW, EdgeW>;
    friend class NodeT< GraphW, NodeW, EdgeW>;
    friend class GraphW;
    friend class NodeW;
public:
    
    enum {Type = TypeEdge};

    int type() const
    {
        return Type;
    }

    void adjust();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
};
#endif