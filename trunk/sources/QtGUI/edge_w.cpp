/**
 * File: edge_w.cpp - Edge Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

void EdgeControl::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        EdgeControl* srcControl = NULL;
        EdgeControl* dstControl = NULL;
        
        if( IsNotNullP( predSeg))
        {
            srcControl = predSeg->src();
            scene()->removeItem( predSeg);
            //delete predSeg;
        }
        if( IsNotNullP( succSeg))
        {
            dstControl = succSeg->dst();
            scene()->removeItem( succSeg);
            //delete succSeg;    
        }
        if( srcControl && dstControl)
        {
            EdgeSegment *seg = new EdgeSegment( edge, srcControl, dstControl);
            scene()->addItem( seg);
        }
        event->ignore();
        scene()->removeItem( this);
        //delete this;
        return;
    } else
    {
        QGraphicsItem::mouseDoubleClickEvent(event);
    }
}

QVariant EdgeControl::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        if( IsNotNullP( edge))
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
    sourcePoint = mapFromItem(GetPred(), 0, 0);
    destPoint = mapFromItem(GetSucc(), 0, 0);
    QPointF nextToSrc = destPoint;
    QPointF nextToDst = sourcePoint;
    bool nextToSrcSelected = false;

    topLeft = sourcePoint;
    bottomRight = destPoint;
    foreach( EdgeControl* control, controls)
    {
        QPointF cpos = mapFromScene(control->pos());
        if ( !nextToSrcSelected)
        {
            nextToSrc = cpos;
            nextToSrcSelected = true;
        }
        nextToDst = cpos;
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
    qreal length = line.length();
    QPointF edgeOffsetSrc((line.dx() * 10) / length, (line.dy() * 10) / length);
    QLineF line2( nextToDst, destPoint);
    length = line2.length();
    QPointF edgeOffsetDst((line2.dx() * 10) / length, (line2.dy() * 10) / length);

    sourcePoint = line.p1() + edgeOffsetSrc;
    destPoint = line2.p2() - edgeOffsetDst;
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

    QPainterPath path( sourcePoint);
    foreach( EdgeControl *control, controls)
    {
        path.lineTo( mapFromScene(control->pos()));
        curr_point = mapFromScene(control->pos());
    }
    path.lineTo( destPoint);
    line.setP1( curr_point);
    line.setP2( destPoint);
    // Draw the line itself
    if( option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawPath(path);

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

void EdgeW::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        EdgeControl* control = new EdgeControl( this);
        this->GetGraph()->scene()->addItem( control);
        control->setPos( ev->pos());
        controls << control;
        points << new QPointF( mapFromItem( control, 0, 0));
    }
    QGraphicsItem::mouseReleaseEvent( ev);
}

void EdgeW::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{

}

void EdgeW::focusInEvent(QFocusEvent *event)
{
    foreach( EdgeControl* control, controls)
    {
        control->setVisible( true);
    }
    QGraphicsItem::focusInEvent( event);
}

void EdgeW::focusOutEvent(QFocusEvent *event)
{
    foreach( EdgeControl* control, controls)
    {
        control->setVisible( false);
    }
    QGraphicsItem::focusOutEvent( event);
}
