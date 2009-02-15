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
    return QRectF( pos(), pos());
}

QPainterPath 
NodeW::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path; 
}

void 
NodeW::paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}