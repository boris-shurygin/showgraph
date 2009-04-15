/**
 * File: edge_control.cpp - EdgeControl class implementation.
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
         if ( IsNotNullP( edge) 
              && edge->mode() == EdgeItem::ModeEdit)
         {
            edge->hideControls();
            edge->setMode( EdgeItem::ModeShow);
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

EdgeControl::EdgeControl( EdgeItem* e, QGraphicsScene* scene):
        QObject(),
        QGraphicsItem( 0, scene),
        edge( e),
        isFixed( false)
{
    setFlag( ItemIsMovable);
    //setFlag(ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
    setPred( NULL);
    setSucc( NULL);
}

QRectF EdgeControl::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -EdgeControlSize - adjust, -EdgeControlSize - adjust,
                  2*( EdgeControlSize + adjust), 2*( EdgeControlSize + adjust));
}

QPainterPath EdgeControl::shape() const
{
    QPainterPath path;
    path.addEllipse( -EdgeControlSize, -EdgeControlSize, 2*EdgeControlSize, 2*EdgeControlSize);
    return path; 
}

void EdgeControl::paint( QPainter *painter,
                         const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if( fixed())
    {
    
    } else
    {
        if (option->state & QStyle::State_Sunken) {
            painter->setBrush( Qt::gray);
            painter->setPen( QPen( Qt::black, 0));
        } else
        {
            painter->setBrush( Qt::lightGray);
            painter->setPen( QPen(Qt::darkGray, 0));
        }
        painter->drawEllipse(-EdgeControlSize, -EdgeControlSize,
                              2*EdgeControlSize, 2*EdgeControlSize);
    }
}

void EdgeControl::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent( event);
}

void EdgeControl::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}