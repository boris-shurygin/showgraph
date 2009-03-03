/**
 * File: edge_w.cpp - Edge Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

void EdgeControl::prepareRemove()
{
    EdgeControl* srcControl = NULL;
    EdgeControl* dstControl = NULL;
    
    if( IsNotNullP( predSeg))
    {
        srcControl = predSeg->src();
    }

    if( IsNotNullP( succSeg))
    {
        dstControl = succSeg->dst();
    }

    if( srcControl && dstControl)
    {
        EdgeSegment *seg = new EdgeSegment( edge, srcControl, dstControl, scene());
    }
    this->deleteLater();
}

EdgeControl::~EdgeControl()
{
    if( IsNotNullP( predSeg))
    {
        delete predSeg;
    }
    if( IsNotNullP( succSeg))
    {
        delete succSeg;    
    }
    prepareGeometryChange();
    update();
}

void EdgeControl::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        //prepareRemove();
    } else if ( event->button() & Qt::LeftButton)
    {
        if( !fixed())
        {
            setFixed( true);
        }
    } else
    {
        QGraphicsItem::mouseDoubleClickEvent(event);
    }
}

QVariant EdgeControl::itemChange(GraphicsItemChange change, const QVariant &value)
{
    
    switch (change) {
    case ItemPositionHasChanged:
        if( IsNotNullP( edge) 
            && this != edge->srcCtrl()
            && this != edge->dstCtrl())
            edge->adjust();
        if( IsNotNullP( predSeg))
        {
            predSeg->adjust();
            EdgeSegment* seg = predSeg->src()->pred();
            if( IsNotNullP( seg))
            {
                seg->adjust();
            }
        }
        if( IsNotNullP( succSeg))
        {
            succSeg->adjust();
            EdgeSegment* seg = succSeg->dst()->succ();
            if( IsNotNullP( seg))
            {
                seg->adjust();
            }
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}  


void EdgeW::adjust()
{
    sourcePoint = mapFromItem(GetPred(), GetPred()->boundingRect().center());
    destPoint = mapFromItem(GetSucc(), GetSucc()->boundingRect().center());
    srcControl->setPos( mapToScene( sourcePoint));
    dstControl->setPos( mapToScene( destPoint));
    QPointF nextToSrc = destPoint;
    QPointF nextToDst = sourcePoint;
    topLeft = sourcePoint;
    bottomRight = destPoint;
    if ( IsNotNullP( srcControl))
    {
        Assert( srcControl->succ());
        nextToSrc = mapFromScene( srcControl->succ()->dst()->pos());
    }
    if ( IsNotNullP( dstControl))
    {
        Assert( dstControl->pred());
        nextToDst = mapFromScene( dstControl->pred()->src()->pos());
    }
    foreach( EdgeControl* control, controls)
    {
        QPointF cpos = mapFromScene(control->pos());
        if ( topLeft.x() > cpos.x())
            topLeft.setX( cpos.x());
        if ( topLeft.y() > cpos.y())
            topLeft.setY( cpos.y());
        if ( bottomRight.x() < cpos.x())
            bottomRight.setX( cpos.x());
        if ( bottomRight.y() < cpos.y())
            bottomRight.setY( cpos.y());
    }
    QLineF line( sourcePoint, nextToSrc);
    QPolygonF endPolygon = mapFromItem( GetPred(), GetPred()->boundingRect());
    QPointF p1 = endPolygon.first();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
             polyLine.intersect( line, &sourcePoint);
        if (intersectType == QLineF::BoundedIntersection)
             break;
        p1 = p2;
    }

    QLineF line2( nextToDst, destPoint);
    endPolygon = mapFromItem( GetSucc(), GetSucc()->boundingRect());
    p1 = endPolygon.first();
    p2;
    
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
             polyLine.intersect( line2, &destPoint);
        if (intersectType == QLineF::BoundedIntersection)
             break;
        p1 = p2;
    }
    srcControl->setPos( mapToScene( sourcePoint));
    dstControl->setPos( mapToScene( destPoint));
    prepareGeometryChange();
}

QRectF 
EdgeW::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(topLeft, QSizeF(bottomRight.x() - topLeft.x(),
                                      bottomRight.y() - topLeft.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath 
EdgeW::shape() const
{
    QPainterPath path( sourcePoint);
    foreach( EdgeControl *control, controls)
    {
        path.lineTo( mapFromScene(control->pos()));
    }
    path.lineTo( destPoint);
    return path; 
}

void 
EdgeW::paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget)
{
    QPointF curr_point;
    QLineF line = QLineF();
    curr_point = sourcePoint;
    QPointF nextToDst = sourcePoint;
    
    if ( IsNotNullP( dstControl))
    {
        Assert( dstControl->pred());
        nextToDst = mapFromScene( dstControl->pred()->src()->pos());
    }

    line.setP1( nextToDst);
    line.setP2( destPoint);
    // Draw the line itself
    if( option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    //painter->drawPath(path);

    // Draw the arrows if there's enough room
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2); 
}
void EdgeW::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
void EdgeW::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    update();
    QGraphicsItem::mouseReleaseEvent( ev);
}

void EdgeW::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        
    }
}

void EdgeW::addControl( EdgeControl* control)
{
    controls << control;
}

void EdgeW::initControls()
{
    srcControl = new EdgeControl( this, scene());
    dstControl = new EdgeControl( this, scene());
    new EdgeSegment( this, srcControl, dstControl, scene());
    addControl( srcControl);
    addControl( dstControl);
    srcControl->setFixed();
    dstControl->setFixed();
    adjust();
}

void EdgeW::showControls()
{
    foreach( EdgeControl* control, controls)
    {
        if ( control != srcControl
             && control != dstControl)
        {
            control->setFixed( false);
        }
    }
}

void EdgeW::hideControls()
{
    foreach( EdgeControl* control, controls)
    {
        control->setFixed( true);
    }
}
