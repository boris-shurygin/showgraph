/**
 * File: edge_w.h - Edge Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_W_H
#define EDGE_W_H

#define EdgeControlSize 5

class EdgeSegment;

class EdgeControl: public QObject, public QGraphicsItem
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

    EdgeControl( EdgeW* e, QGraphicsScene* scene): QObject(), QGraphicsItem( 0, scene), edge( e)
    {
        setFlag( ItemIsMovable);
        //setFlag(ItemIsSelectable);
        setCacheMode(DeviceCoordinateCache);
        setZValue(1);
        setPred( NULL);
        setSucc( NULL);
    }

    ~EdgeControl();

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
        QGraphicsItem::mousePressEvent( event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        update();
        QGraphicsItem::mouseReleaseEvent(event);
    }
    
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
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
    void adjust()
    {
        srcP = mapFromItem( srcControl, 0, 0);
        dstP = mapFromItem( dstControl, 0, 0);
        
        topLeft.setX( min< qreal>( srcP.x(), dstP.x()));
        topLeft.setY( min< qreal>( srcP.y(), dstP.y()));
        btmRight.setX( max< qreal>( srcP.x(), dstP.x()));
        btmRight.setY( max< qreal>( srcP.y(), dstP.y()));

        QLineF mainLine = QLineF( srcP, dstP);
        qreal size = Abs< qreal>(( min< qreal>( Abs< qreal>( mainLine.dx()),
                                                Abs< qreal>(mainLine.dy()))));
        //Stub
        if( size < 2* EdgeControlSize)
        {
            size = 2* EdgeControlSize;
        }
        if( size > 20 * EdgeControlSize)
        {
            size = 20 * EdgeControlSize;
        }

        /** Place cp1 */
        if ( IsNotNullP( srcControl->pred()))
        {
            QPointF p1 = mapFromScene( srcControl->pred()->src()->pos());
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
        
        topLeft.setX( min< qreal>( topLeft.x(), cp1.x()));
        topLeft.setY( min< qreal>( topLeft.y(), cp1.y()));
        btmRight.setX( max< qreal>( btmRight.x(), cp1.x()));
        btmRight.setY( max< qreal>( btmRight.y(), cp1.y()));

        /** Place cp2 */
        if ( IsNotNullP( dstControl->succ()))
        {
            QPointF p2 = mapFromScene( dstControl->succ()->dst()->pos());
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
        
        topLeft.setX( min< qreal>( topLeft.x(), cp2.x()));
        topLeft.setY( min< qreal>( topLeft.y(), cp2.y()));
        btmRight.setX( max< qreal>( btmRight.x(), cp2.x()));
        btmRight.setY( max< qreal>( btmRight.y(), cp2.y()));

        prepareGeometryChange();
    }

    EdgeSegment( EdgeW *e, EdgeControl *src, EdgeControl* dst, QGraphicsScene* scene): 
        QGraphicsItem( 0, scene),
        edge( e),
        srcControl(src),
        dstControl( dst)
    {
        Assert( IsNotNullP( src));
        Assert( IsNotNullP( dst));
        //setCacheMode(DeviceCoordinateCache);
        //setFlag( ItemIsMovable);
        setZValue(0);
        src->setSucc( this);
        dst->setPred( this);
        adjust();
    }
    
    ~EdgeSegment()
    {
        if( IsNotNullP( src()) && src()->succ() == this)
        {
            src()->setSucc( NULL);
        }
        if( IsNotNullP( dst()) && dst()->pred() == this)
        {
            dst()->setPred( NULL);
        }
        prepareGeometryChange();
        scene()->removeItem( this);
    }

    QRectF boundingRect() const
    {
        qreal penWidth = 1;
        qreal extra = penWidth / 2.0;

        return QRectF( topLeft,
                       QSizeF( btmRight.x() - topLeft.x(),
                               btmRight.y() - topLeft.y()))
               .normalized()
               .adjusted(-extra, -extra, extra, extra);
    }

    QPainterPath shape() const
    {
        QPainterPath path( srcP);
        QPainterPathStroker stroker;
        path.cubicTo( cp1, cp2, dstP);
        stroker.setWidth( 2);
        return stroker.createStroke( path); 
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QPainterPath path( srcP);
        QPainterPathStroker stroker;
        path.cubicTo( cp1, cp2, dstP);
        
        // Select the pen
        if( option->state & QStyle::State_Selected)
        {
            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
        {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        
        stroker.setWidth( 4);
        
        painter->drawPath(path);
        
        /** For illustrative purposes */
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        //painter->drawPath( stroker.createStroke( path));
        //painter->drawLine( srcP, dstP);
        //painter->drawLine( srcP, cp1);
        //painter->drawLine( cp2, dstP);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        update();
        if ( event->button() & Qt::RightButton)
        {
            adjust();
        }
        if (event->button() == Qt::LeftButton && (flags() & ItemIsSelectable)) {
            bool multiSelect = (event->modifiers() & Qt::ControlModifier) != 0;
            if (!multiSelect) {
                if (!isSelected()) {
                    if (scene())
                        scene()->clearSelection();
                    setSelected(true);
                }
            }
        } else if (!(flags() & ItemIsMovable)) {
//            event->ignore();
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        update();
        if ( event->button() & Qt::RightButton)
        {
            adjust();
        }
        QGraphicsItem::mouseReleaseEvent(event);
    }
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        update();
        if ( event->button() & Qt::LeftButton)
        {
            EdgeControl *control = new EdgeControl( edge, scene());
            control->setPos( event->pos());
            EdgeSegment* seg = new EdgeSegment( edge, control, dst(), scene()); 
            setDst( control);
            adjust();
        }
        QGraphicsItem::mouseDoubleClickEvent(event);
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