/**
 * File: graph_w.cpp - Graph Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

/** Contructor */
GraphW::GraphW()
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setSceneRect(-400, -400, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setMinimumSize(400, 400);
    setWindowTitle(tr("ShowGraph"));
}

/** Destructor */
GraphW::~GraphW()
{

}

void 
GraphW::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint( QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

NodeW*
GraphW::NewNode()
{
    NodeW* n = GraphT< GraphW, NodeW, EdgeW>::NewNode();
    scene()->addItem( n);
    return n;
}

EdgeW*
GraphW::NewEdge( NodeW* pred, NodeW* succ)
{
    EdgeW* e = GraphT< GraphW, NodeW, EdgeW>::NewEdge( pred, succ);
    scene()->addItem( e);
    return e;
}
