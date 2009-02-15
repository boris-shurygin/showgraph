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

void 
GraphW::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        QPoint p = ev->pos();
        NodeW* node = NewNode();
        node->setPos( mapToScene( p));
        QGraphicsView::mouseDoubleClickEvent( ev);
    } else if( ev->button() & Qt::RightButton)
    {
        NodeW *node = (NodeW *)scene()->itemAt( mapToScene( ev->pos()));
        if( IsNotNullP( node))
        {
            delete node;
        }
    }
}

void GraphW::mousePressEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::RightButton)
    {
        src = ev->pos();
        createEdge = true;
    }
    QGraphicsView::mousePressEvent( ev);
}

void GraphW::mouseReleaseEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::RightButton)
    {
        dst = ev->pos();
        if ( createEdge)
        {
            NodeW *src_node = (NodeW *)scene()->itemAt( mapToScene( src));
            NodeW *dst_node = (NodeW *)scene()->itemAt( mapToScene( dst));
            if( IsNotNullP( src_node) && IsNotNullP( dst_node))
            {
                NewEdge( src_node, dst_node);
            }
        }
        createEdge = false;
    }
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