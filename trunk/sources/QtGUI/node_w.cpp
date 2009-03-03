/**
 * File: node_w.cpp - Node Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

NodeW::~NodeW()
{

}

QRectF 
NodeW::boundingRect() const
{
    qreal adjust = 5;
    return QGraphicsTextItem::boundingRect()
               .adjusted( -adjust, -adjust, adjust, adjust);
}

QPainterPath 
NodeW::shape() const
{
    QPainterPath path;
    path.addRect( boundingRect());
    return path; 
}

void 
NodeW::paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget)
{
    qreal adjust = 3;
    if (option->state & QStyle::State_Sunken)
    {
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawRect( boundingRect());
    QGraphicsTextItem::paint( painter, option, widget);
}

void NodeW::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( event->button() & Qt::RightButton)
    {
        GetGraph()->SetCreateEdge( true);
        GetGraph()->SetTmpSrc( this);
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void NodeW::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeW::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void NodeW::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

QVariant NodeW::itemChange(GraphicsItemChange change, const QVariant &value)
{
    EdgeW *edge = NULL;

    switch (change) {
    case ItemPositionHasChanged:
        for ( edge = GetFirstSucc(); !EndOfSuccs(); edge = GetNextSucc())
        {
            edge->adjust();
        }
        for ( edge = GetFirstPred(); !EndOfPreds(); edge = GetNextPred())
        {
            edge->adjust();
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
