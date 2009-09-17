/**
 * File: node_w.cpp - Node Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

GraphView * NodeItem::graph() const
{
    return static_cast< GraphView *>( AuxNode::graph());
}

NodeItem::~NodeItem()
{
    removeFromIndex();
    scene()->removeItem( this);
}

QRectF 
NodeItem::boundingRect() const
{
    if ( isEdgeControl())
    {
        qreal adjust = 2;
        return QRectF( -EdgeControlSize - adjust, -EdgeControlSize - adjust,
              2*( EdgeControlSize + adjust), 2*( EdgeControlSize + adjust));
    } else
    {
        qreal adjust = 5;
        return QGraphicsTextItem::boundingRect()
            .adjusted( -adjust, -adjust, adjust, adjust);
    }
}

QPainterPath 
NodeItem::shape() const
{
    if ( isEdgeControl())
    {
        QPainterPath path;
        path.addEllipse( -EdgeControlSize, -EdgeControlSize, 2*EdgeControlSize, 2*EdgeControlSize);
        return path; 
    } else
    {
        QPainterPath path;
        path.addRect( boundingRect());
        return path;
    }
}

void 
NodeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if ( isSimple())
    {
        qreal adjust = 3;
        if (option->state & QStyle::State_Sunken)
        {
            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
        {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        QGraphicsTextItem::paint( painter, option, widget);
        painter->drawRect( boundingRect());
    } else if ( isEdgeControl())
    {
        if (option->state & QStyle::State_Sunken) 
        {
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

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( event->button() & Qt::RightButton && !isEdgeControl())
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
    if ( event->button() & Qt::LeftButton && !isEdgeControl())
    {
        if ( textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
}

void NodeItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
void  NodeItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);
    EdgeItem *edge = NULL;

    for ( edge = firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
    {
        edge->adjust();
    }
    for ( edge = firstPred(); isNotNullP( edge); edge = edge->nextPred())
    {
        edge->adjust();
    }
}

QVariant NodeItem::itemChange( GraphicsItemChange change, const QVariant &value)
{
    EdgeItem *edge = NULL;

    if ( change != QGraphicsItem::ItemSceneChange)
    {
        for ( edge = firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
        {
            edge->adjust();
        }
        for ( edge = firstPred(); isNotNullP( edge); edge = edge->nextPred())
        {
            edge->adjust();
        }
    }
    return QGraphicsTextItem::itemChange(change, value);
}

/**
 * Update DOM tree element
 */
void
NodeItem::updateElement()
{
    AuxNode::updateElement();// Base class method call
    QDomElement e = elem();
    e.setAttribute( "x", QGraphicsItem::x());
    e.setAttribute( "y", QGraphicsItem::y());
    e.setAttribute( "label", toPlainText());
}

/**
 * read properties from DOM tree element
 */
void
NodeItem::readFromElement( QDomElement e)
{
    assert( !e.isNull());
    assert( e.tagName() == QString( "node"));
    
    if( e.hasAttribute( "x") && e.hasAttribute( "y"))
    {
        setPos( e.attribute( "x").toDouble(),
                e.attribute( "y").toDouble());
    }
    if( e.hasAttribute( "label"))
    {
        setPlainText( e.attribute( "label"));
    }
    AuxNode::readFromElement( e); // Base class method
}