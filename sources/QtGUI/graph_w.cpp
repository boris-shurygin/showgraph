/**
 * File: graph_w.cpp - Graph Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

/** Contructor */
GraphW::GraphW(): dst( 0, 0), src( 0, 0), createEdge( false)
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
    tmpSrc = NULL;
}

/** Destructor */
GraphW::~GraphW()
{

}

void 
GraphW::drawBackground(QPainter *painter, const QRectF &rect)
{

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
    e->initControls();
    return e;
}

void 
GraphW::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        QPoint p = ev->pos();
        if ( !scene()->itemAt( mapToScene( ev->pos())))
        {
            NodeW* node = NewNode();
            node->setPos( mapToScene( p));
        }
    } else if( ev->button() & Qt::RightButton)
    {
        QGraphicsItem *node = scene()->itemAt( mapToScene( ev->pos()));
        if ( IsNotNullP( node) && qgraphicsitem_cast<NodeW *>( node))
        {
            delete qgraphicsitem_cast<NodeW *>( node);
        }
    }
    QGraphicsView::mouseDoubleClickEvent( ev);   
}

void GraphW::mousePressEvent(QMouseEvent *ev)
{
    QGraphicsView::mousePressEvent( ev);
}

void GraphW::mouseReleaseEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::RightButton)
    {
        if ( createEdge)
        {
            QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()));
            if ( IsNotNullP( item) && qgraphicsitem_cast<NodeW *>(item))
            {
                if ( tmpSrc != qgraphicsitem_cast<NodeW *>(item))
                {
                    NewEdge( tmpSrc, qgraphicsitem_cast<NodeW *>(item));
                }
            }
        }
    }
    tmpSrc = NULL;
    createEdge = false;
    QGraphicsView::mouseReleaseEvent(ev);
}

void GraphW::mouseMoveEvent(QMouseEvent *ev)
{
    if ( createEdge)
    {
        dst = ev->pos();
    }
    QGraphicsView::mouseMoveEvent(ev);
}

void GraphW::drawForeground(QPainter *painter, const QRectF &rect)
{

}