/**
 * File: edge_w.cpp - Edge Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"
#include <QVector>

NodeItem * 
EdgeItem::node( GraphDir dir) const 
{
    return static_cast< NodeItem *>(AuxEdge::node( dir));
}

void
EdgeItem::adjust()
{
    sourcePoint = mapFromItem( pred(), pred()->boundingRect().center());
    destPoint = mapFromItem( succ(), succ()->boundingRect().center());
    topLeft = sourcePoint;
    bottomRight = destPoint;
    if ( pred()->isEdgeControl())
    {

    } else
    {
        QLineF line( sourcePoint, destPoint);
        QPolygonF endPolygon = mapFromItem( pred(), pred()->boundingRect());
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
    }
    if ( succ()->isEdgeControl())
    {
    
    } else
    {
        QLineF line2( sourcePoint, destPoint);
        QPolygonF endPolygon = mapFromItem( succ(), succ()->boundingRect());
        QPointF p1 = endPolygon.first();;
        QPointF p2;
        QLineF polyLine;
        
        for ( int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i);
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                 polyLine.intersect( line2, &destPoint);
            if ( intersectType == QLineF::BoundedIntersection)
                 break;
            p1 = p2;
        }
    }
    prepareGeometryChange();
}

QRectF 
EdgeItem::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(topLeft, QSizeF(bottomRight.x() - topLeft.x(),
                                      bottomRight.y() - topLeft.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath 
EdgeItem::shape() const
{
    QPainterPath path( sourcePoint);
    QPainterPathStroker stroker;
    path.lineTo( destPoint);
    stroker.setWidth( 2);
    return stroker.createStroke( path); 
}

void 
EdgeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    QPointF curr_point;
    QLineF line = QLineF();
    curr_point = sourcePoint;
    QPointF nextToDst = sourcePoint;
    
    line.setP1( nextToDst);
    line.setP2( destPoint);

    if ( nextToDst == destPoint)
        return;
    // Draw the line itself
    if( option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    // Draw the arrows if there's enough room
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawLine( line);
    if ( !succ()->isEdgeControl())
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2); 
}
void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        adjust();
    }
    if ( event->button() == Qt::LeftButton && (flags() & ItemIsSelectable))
    {
        bool multiSelect = ( event->modifiers() & Qt::ControlModifier) != 0;
        if ( !multiSelect)
        {
            if ( !isSelected())
            {
                if (scene())
                    scene()->clearSelection();
                setSelected(true);
            }
        }
    } else if ( !(flags() & ItemIsMovable))
    {
//            event->ignore();
    }
}
void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    update();
    QGraphicsItem::mouseReleaseEvent( ev);
}

void EdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        NodeItem* new_node = static_cast<NodeItem *>( insertNode());
        new_node->setTypeEdgeControl();
        new_node->setPos( ev->pos());
    }
}

/**
 * Update DOM tree element
 */
void
EdgeItem::updateElement()
{
    QDomElement e = elem();
    int i = 0;
    QDomElement new_e = graph()->createElement( "edge");
    QDomNode e2 = graph()->documentElement().removeChild( e);
    assert( !e2.isNull());
    graph()->documentElement().appendChild( new_e);
    setElement( new_e);
    e = new_e;

    /* Base class method call to print generic edge properties */
    AuxEdge::updateElement();
}

/**
 * read properties from DOM tree element
 */
void
EdgeItem::readFromElement( QDomElement e)
{
    AuxEdge::readFromElement( e); // Base class method
    adjust();
}
