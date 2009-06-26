/**
 * File: edge_seg.cpp - EdgeSegment class implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

void EdgeSegment::adjust()
{
    srcP = mapFromItem( srcControl, 0, 0);
    dstP = mapFromItem( dstControl, 0, 0);
    
    topLeft.setX( min< qreal>( srcP.x(), dstP.x()));
    topLeft.setY( min< qreal>( srcP.y(), dstP.y()));
    btmRight.setX( max< qreal>( srcP.x(), dstP.x()));
    btmRight.setY( max< qreal>( srcP.y(), dstP.y()));

    QLineF mainLine = QLineF( srcP, dstP);
        
    if ( mainLine.length() < 1)
        return;

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
    if ( isNotNullP( srcControl->pred()))
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
    if ( isNotNullP( dstControl->succ()))
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

EdgeSegment::EdgeSegment( EdgeItem *e, EdgeControl *src, EdgeControl* dst, QGraphicsScene* scene): 
    QGraphicsItem( 0, scene),
    edge( e),
    srcControl(src),
    dstControl( dst)
{
    assert( isNotNullP( src));
    assert( isNotNullP( dst));
    //setCacheMode(DeviceCoordinateCache);
    //setFlag( ItemIsMovable);
    setZValue(0);
    src->setSucc( this);
    dst->setPred( this);
    adjust();
}

EdgeSegment::~EdgeSegment()
{
    if( isNotNullP( src()) && src()->succ() == this)
    {
        src()->setSucc( NULL);
    }
    if( isNotNullP( dst()) && dst()->pred() == this)
    {
        dst()->setPred( NULL);
    }
    removeFromIndex();
    update();
}

QRectF EdgeSegment::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = penWidth / 2.0;

    return QRectF( topLeft,
                   QSizeF( btmRight.x() - topLeft.x(),
                           btmRight.y() - topLeft.y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

QPainterPath EdgeSegment::shape() const
{
    QPainterPath path( srcP);
        
    if ( srcP == dstP)
        return path;
    
    QPainterPathStroker stroker;
    path.cubicTo( cp1, cp2, dstP);
    stroker.setWidth( 2);
    return stroker.createStroke( path); 
}

void EdgeSegment::paint( QPainter *painter,
                         const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path( srcP);
    QPainterPathStroker stroker;
        
    if ( srcP == dstP)
    {
        return;
    }
    path.cubicTo( cp1, cp2, dstP);
    
    // Select the pen
    if ( edge->isInverted())
    {
        painter->setPen(QPen( Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        if( isNotNullP( edge) && edge->mode() == EdgeItem::ModeEdit)
        {
            painter->setPen(QPen( Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
        {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }    
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

void EdgeSegment::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void EdgeSegment::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        adjust();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

EdgeControl *EdgeSegment::addControl( QPointF p)
{
    EdgeControl *control = new EdgeControl( edge, scene());
    control->setPos( p);
    EdgeSegment* seg = new EdgeSegment( edge, control, dst(), scene()); 
    setDst( control);
    
    if ( isNotNullP( edge))
    {
        edge->addControl( control);    
    }
    adjust();
    return control;
}

EdgeControl *EdgeSegment::addControl( EdgeControl *control)
{
    EdgeSegment* seg = new EdgeSegment( edge, control, dst(), scene()); 
    setDst( control);
    
    if ( isNotNullP( edge))
    {
        edge->addControl( control);    
    }
    adjust();
    return control;
}

void EdgeSegment::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( isNotNullP( edge) 
         && event->button() & Qt::LeftButton)
    { 
        if ( edge->mode() == EdgeItem::ModeShow)
        {
            edge->showControls();
            edge->setMode( EdgeItem::ModeEdit);
        } else if ( edge->mode() == EdgeItem::ModeEdit)
        {
            addControl( event->pos());
        }
    }
    edge->adjust();
    QGraphicsItem::mouseDoubleClickEvent(event);
}
