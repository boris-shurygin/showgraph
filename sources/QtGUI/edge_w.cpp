/**
 * File: edge_w.cpp - Edge Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

QRectF 
EdgeW::boundingRect() const
{
    return QRectF( GetPred()->pos(), GetSucc()->pos());
}

QPainterPath 
EdgeW::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path; 
}

void 
EdgeW::paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget)
{

}