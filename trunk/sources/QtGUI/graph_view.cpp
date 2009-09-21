/**
 * File: graph_w.cpp - Graph Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

/** Contructor */
GraphView::GraphView(): dst( 0, 0), src( 0, 0), createEdge( false)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setSceneRect(0, 0, 100, 100);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setMinimumSize(200, 200);
    setWindowTitle(tr("ShowGraph"));
    tmpSrc = NULL;
}

/** Destructor */
GraphView::~GraphView()
{
    for ( NodeItem *node = firstNode();
          isNotNullP( node);
          )
    {
        NodeItem* next = node->nextNode();
        delete node;
        node = next;
    }
}

void 
GraphView::drawBackground(QPainter *painter, const QRectF &rect)
{

}

NodeItem*
GraphView::newNode()
{
    NodeItem* n = static_cast< NodeItem *>( AuxGraph::newNode());
    scene()->addItem( n);
    return n;
}

EdgeItem*
GraphView::newEdge( NodeItem* pred, NodeItem* succ)
{
    EdgeItem* e = 
        static_cast< EdgeItem *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ));
    scene()->addItem( e);
    e->adjust();
    return e;
}

NodeItem*
GraphView::newNode( QDomElement e)
{
    NodeItem* n =  static_cast< NodeItem *>( AuxGraph::newNode( e));
    scene()->addItem( n);
    return n;
}

EdgeItem*
GraphView::newEdge( NodeItem* pred, NodeItem* succ, QDomElement e)
{
    EdgeItem* edge_p = 
        static_cast< EdgeItem *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ, e));
    scene()->addItem( edge_p);
    edge_p->adjust();
    return edge_p;
}

void 
GraphView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        QPoint p = ev->pos();
        if ( !scene()->itemAt( mapToScene( ev->pos())))
        {
            NodeItem* node = newNode();
            node->setPos( mapToScene( p));
        }
    } else if( ev->button() & Qt::RightButton)
    {
        QGraphicsItem *node = scene()->itemAt( mapToScene( ev->pos()));
        if ( isNotNullP( node) && qgraphicsitem_cast<NodeItem *>( node))
        {
            delete qgraphicsitem_cast<NodeItem *>( node);
        }
    }
    QGraphicsView::mouseDoubleClickEvent( ev);   
}

void GraphView::mousePressEvent(QMouseEvent *ev)
{
    QGraphicsView::mousePressEvent( ev);
}

void GraphView::mouseReleaseEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::RightButton)
    {
        if ( createEdge)
        {
            QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()));
            if ( isNotNullP( item) && qgraphicsitem_cast<NodeItem *>(item))
            {
                if ( tmpSrc != qgraphicsitem_cast<NodeItem *>(item))
                {
                    newEdge( tmpSrc, qgraphicsitem_cast<NodeItem *>(item));
                }
            }
        }
    }
    tmpSrc = NULL;
    createEdge = false;
    QGraphicsView::mouseReleaseEvent(ev);
}

void GraphView::mouseMoveEvent(QMouseEvent *ev)
{
    if ( createEdge)
    {
        dst = ev->pos();
    }
    QGraphicsView::mouseMoveEvent(ev);
}

void GraphView::drawForeground( QPainter *painter, const QRectF &rect)
{

}
