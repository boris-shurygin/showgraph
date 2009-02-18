/**
 * File: edge_w.cpp - Edge Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

void EdgeW::adjust()
{
    QLineF line(mapFromItem(GetPred(), 0, 0), mapFromItem(GetSucc(), 0, 0));
    qreal length = line.length();
    QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);

    sourcePoint = line.p1() + edgeOffset;
    destPoint = line.p2() - edgeOffset;
    prepareGeometryChange();
}

QRectF 
EdgeW::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath 
EdgeW::shape() const
{
    QPainterPath path( sourcePoint);
    path.lineTo( destPoint);
    return path; 
}

void 
EdgeW::paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget)
{
    QLineF line = QLineF();
    line.setP1( sourcePoint);
    line.setP2( destPoint);

    // Draw the line itself
    if( option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);

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