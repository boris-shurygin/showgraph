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
    
    if( isNotNullP( predSeg))
    {
        srcControl = predSeg->src();
        scene()->removeItem( predSeg);
    }

    if( isNotNullP( succSeg))
    {
        dstControl = succSeg->dst();
        scene()->removeItem( succSeg);
    }

    if( srcControl && dstControl)
    {
        EdgeSegment *seg = new EdgeSegment( edge, srcControl, dstControl, scene());
    }
    removeFromIndex();
    scene()->removeItem( this);
    edge->removeControl( this);
    edge->adjust();
    deleteLater();
}

EdgeControl::~EdgeControl()
{
    removeFromIndex();
    if( isNotNullP( predSeg))
    {
        delete predSeg;
    }
    if( isNotNullP( succSeg))
    {
        delete succSeg;    
    }
}

void EdgeControl::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        prepareRemove();
    } else if ( event->button() & Qt::LeftButton)
    {
         if ( isNotNullP( edge) 
              && edge->mode() == EdgeItem::ModeEdit)
         {
            edge->hideControls();
            edge->setMode( EdgeItem::ModeShow);
         }
    } else
    {
        QGraphicsItem::mouseDoubleClickEvent(event);
    }
    edge->adjust();
}

QVariant EdgeControl::itemChange(GraphicsItemChange change, const QVariant &value)
{
    
    switch (change) {
    case ItemPositionHasChanged:
        if( isNotNullP( edge) 
            && this != edge->srcCtrl()
            && this != edge->dstCtrl())
            edge->adjust();
        if( isNotNullP( predSeg))
        {
            predSeg->adjust();
            EdgeSegment* seg = predSeg->src()->pred();
            if( isNotNullP( seg))
            {
                seg->adjust();
            }
        }
        if( isNotNullP( succSeg))
        {
            succSeg->adjust();
            EdgeSegment* seg = succSeg->dst()->succ();
            if( isNotNullP( seg))
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