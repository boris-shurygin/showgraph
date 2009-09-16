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
    srcControl->setPos( mapToScene( sourcePoint));
    dstControl->setPos( mapToScene( destPoint));
    QPointF nextToSrc = destPoint;
    QPointF nextToDst = sourcePoint;
    topLeft = sourcePoint;
    bottomRight = destPoint;
    if ( isNotNullP( srcControl))
    {
        assert( srcControl->succ());
        nextToSrc = mapFromScene( srcControl->succ()->dst()->pos());
    }
    if ( isNotNullP( dstControl))
    {
        assert( dstControl->pred());
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

    QLineF line2( nextToDst, destPoint);
    endPolygon = mapFromItem( succ(), succ()->boundingRect());
    p1 = endPolygon.first();
    p2;
    
    for ( int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
             polyLine.intersect( line2, &destPoint);
        if ( intersectType == QLineF::BoundedIntersection)
             break;
        p1 = p2;
    }
    srcControl->setPos( mapToScene( sourcePoint));
    dstControl->setPos( mapToScene( destPoint));
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
    /*QPainterPath path( sourcePoint);
    foreach( EdgeControl *control, controls)
    {
        path.lineTo( mapFromScene(control->pos()));
    }
    path.lineTo( destPoint);*/
    return QPainterPath(); 
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
    
    if ( isNotNullP( dstControl))
    {
        assert( dstControl->pred());
        nextToDst = mapFromScene( dstControl->pred()->src()->pos());
    }

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
        
    }
}

void EdgeItem::addControl( EdgeControl* control)
{
    controls << control;
}

void EdgeItem::removeControl( EdgeControl* control)
{
    controls.removeOne( control);
}

void EdgeItem::initControls()
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

void EdgeItem::removeControls()
{
    foreach( EdgeControl* control, controls)
    {
        if ( control != srcControl
             && control != dstControl)
        {
            control->prepareRemove();
        }
    }   
}

void EdgeItem::showControls()
{
    foreach( EdgeControl* control, controls)
    {
        if ( control != srcControl
             && control != dstControl)
        {
            control->setFixed( false);
            control->update();
        }
    }
}

void EdgeItem::hideControls()
{
    foreach( EdgeControl* control, controls)
    {
        control->setFixed( true);
        control->update();
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
    EdgeControl* src_ctrl = srcCtrl();
    EdgeControl* dst_ctrl = dstCtrl();
    
    /* Save the control points of the edge */
    for( EdgeControl* control = src_ctrl->succ()->dst();
         control != dst_ctrl;
         control = control->succ()->dst(), i++)
    {
        QDomElement point_elem = graph()->createElement( "point");
        e.appendChild( point_elem);
        point_elem.setAttribute( "id", i);
        point_elem.setAttribute( "x", control->x());
        point_elem.setAttribute( "y", control->y());
    }
    e.setAttribute("points_num", i);
}

/**
 * read properties from DOM tree element
 */
void
EdgeItem::readFromElement( QDomElement e)
{
    AuxEdge::readFromElement( e); // Base class method
    QDomNode n = e.firstChild();
    QVector< EdgeControl *> points;
    points.resize( e.attribute("points_num").toUInt());
    EdgeSegment* seg = srcCtrl()->succ();

    while ( !n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        
        if ( !e.isNull() && e.tagName() == QString( "point"))
        {
            EdgeControl* ctrl = new EdgeControl( this, scene());
            ctrl->setPos( e.attribute("x").toDouble(),
                          e.attribute("y").toDouble());
            points[ e.attribute("id").toUInt()] = ctrl;
        }
        n = n.nextSibling();
    }
    for ( unsigned int i =0; i < e.attribute("points_num").toUInt(); i++)
    {
        seg->addControl( points[ i]); // Divide segment by control
        seg = seg->dst()->succ(); // Move on to newly created segment
    }
    hideControls();
    adjust();
}
