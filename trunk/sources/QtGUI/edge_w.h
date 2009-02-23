/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

#define EdgeControlSize 5

class EdgeSegment;

class EdgeControl: public QGraphicsItem
{
    EdgeW *edge; 
    EdgeSegment* predSeg;
    EdgeSegment* succSeg;
public: 
        
    
    enum {Type = TypeEdgeControl};

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

    EdgeControl( EdgeW* e): QGraphicsItem(), edge( e)
    {
        setFlag( ItemIsMovable);
        //setFlag(ItemIsSelectable);
        setCacheMode(DeviceCoordinateCache);
        setZValue(1);
        setPred( NULL);
        setSucc( NULL);
    }
    QRectF boundingRect() const
    {
        qreal adjust = 2;
        return QRectF(-EdgeControlSize - adjust, -EdgeControlSize - adjust,
                      2*( EdgeControlSize + adjust), 2*( EdgeControlSize + adjust));
    }

    QPainterPath shape() const
    {
        QPainterPath path;
        path.addEllipse(-EdgeControlSize, -EdgeControlSize, 2*EdgeControlSize, 2*EdgeControlSize);
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
                              2*EdgeControlSize, 2*EdgeControlSize);
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

class EdgeSegment: public QGraphicsItem
{
    EdgeW* edge;
    QPointF srcP;
    QPointF cp1;
    QPointF cp2;
    QPointF dstP;
    EdgeControl* srcControl;
    EdgeControl* dstControl;

    enum SegmentType
    {
        LineSegment,
        CurveSegment
    };

public:
    
    inline QPointF src() const
    {
        return srcP;
    }

    inline QPointF dst() const
    {
        return dstP;
    }

    void adjust()
    {
        srcP = mapFromItem( srcControl, 0, 0);
        dstP = mapFromItem( dstControl, 0, 0);
        QLineF mainLine = QLineF( srcP, dstP);
        qreal size = Abs< qreal>(( min< qreal>( Abs< qreal>( mainLine.dx()),
                                                Abs< qreal>(mainLine.dy()))));
        
        /** Place cp1 */
        if ( IsNotNullP( srcControl->pred()))
        {
            QPointF p1 = mapFromItem( srcControl->pred(), srcControl->pred()->src());
            QLineF line( p1, dstP);
            QPointF cp1_offset = QPointF( (line.dx() * size)/ line.length(),
                                          (line.dy() * size)/ line.length());
            cp1 = srcP + cp1_offset;
        } else
        {
            QPointF cp1_offset = QPointF( (mainLine.dx() * size)/ mainLine.length(),
                                          (mainLine.dy() * size)/ mainLine.length());
            cp1 = srcP + cp1_offset;
        }
        
        /** Place cp2 */
        if ( IsNotNullP( srcControl->succ()))
        {
            QPointF p2 = mapFromItem( srcControl->succ(), srcControl->succ()->dst());
            QLineF line( p2, srcP);
            QPointF cp2_offset = QPointF( (line.dx() * size)/ line.length(),
                                          (line.dy() * size)/ line.length());
            cp2 = dstP + cp2_offset;
        } else
        {
            QPointF cp2_offset = QPointF( -(mainLine.dx() * size)/ mainLine.length(),
                                          -(mainLine.dy() * size)/ mainLine.length());
            cp2 = dstP + cp2_offset;
        }
        prepareGeometryChange();
    }

    EdgeSegment( EdgeW *e, EdgeControl *src, EdgeControl* dst): 
        QGraphicsItem(),
        edge( e),
        srcControl(src),
        dstControl( dst)
    {
        Assert( IsNotNullP( src));
        Assert( IsNotNullP( dst));
        src->setSucc( this);
        dst->setPred( this);
        adjust();
    }


    QRectF boundingRect() const
    {
        qreal penWidth = 1;
        qreal extra = penWidth / 2.0;

        return QRectF( srcP, QSizeF(dstP.x() - srcP.x(), dstP.y() - srcP.y()))
               .normalized()
               .adjusted(-extra, -extra, extra, extra);
    }

    QPainterPath shape() const
    {
        QPainterPath path( srcP);
        path.cubicTo( cp1, cp2, dstP);
        return path; 
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QPainterPath path( srcP);
        path.cubicTo( cp1, cp2, dstP);
        
        // Draw the line itself
        if( option->state & QStyle::State_Selected)
        {
            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
        {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        
        painter->drawPath(path);
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine( srcP, dstP);
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