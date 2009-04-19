/**
 * File: node_w.cpp - Node Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

NodeItem::~NodeItem()
{

}

QRectF 
NodeItem::boundingRect() const
{
    qreal adjust = 5;
    return QGraphicsTextItem::boundingRect()
               .adjusted( -adjust, -adjust, adjust, adjust);
}

QPainterPath 
NodeItem::shape() const
{
    QPainterPath path;
    path.addRect( boundingRect());
    return path; 
}

void 
NodeItem::paint( QPainter *painter,
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

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( event->button() & Qt::RightButton)
    {
        graph()->SetCreateEdge( true);
        graph()->SetTmpSrc( this);
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void NodeItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    EdgeItem *edge = NULL;

    switch (change) {
    case ItemPositionHasChanged:
        for ( edge = firstSucc(); !endOfSuccs(); edge = nextSucc())
        {
            edge->adjust();
        }
        for ( edge = firstPred(); !endOfPreds(); edge = nextPred())
        {
            edge->adjust();
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
