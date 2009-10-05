/**
 * File: graph_w.cpp - Graph Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

/** Destructor */
GGraph::~GGraph()
{
    for ( GNode *node = firstNode();
          isNotNullP( node);
          )
    {
        GNode* next = node->nextNode();
        delete node;
        node = next;
    }
}

GNode*
GGraph::newNode()
{
    GNode* n = static_cast< GNode *>( AuxGraph::newNode());
    view()->scene()->addItem( n->item());
    return n;
}

GEdge*
GGraph::newEdge( GNode* pred, GNode* succ)
{
    GEdge* e = 
        static_cast< GEdge *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ));
    view()->scene()->addItem( e->item());
    e->item()->adjust();
    return e;
}

GNode*
GGraph::newNode( QDomElement e)
{
    GNode* n =  static_cast< GNode *>( AuxGraph::newNode( e));
    view()->scene()->addItem( n->item());
    return n;
}

GEdge*
GGraph::newEdge( GNode* pred, GNode* succ, QDomElement e)
{
    GEdge* edge_p = 
        static_cast< GEdge *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ, e));
    view()->scene()->addItem( edge_p->item());
    edge_p->item()->adjust();
    return edge_p;
}


/** Constructor */
GraphView::GraphView(): 
    dst( 0, 0), src( 0, 0),
    createEdge( false),
    graph_p( new GGraph( this)),
	zoom_scale( 0)
{
    QGraphicsScene *scene = new QGraphicsScene( this);
    //scene->setItemIndexMethod( QGraphicsScene::NoIndex);
    //scene->setSceneRect(0, 0, 10000, 10000);
    setScene( scene);
    //setCacheMode( CacheBackground);
    setViewportUpdateMode( SmartViewportUpdate);
    setRenderHint( QPainter::Antialiasing);
    setTransformationAnchor( AnchorUnderMouse);
    setResizeAnchor( AnchorViewCenter);
    setMinimumSize( 200, 200);
    setWindowTitle( tr("ShowGraph"));
    //setDragMode( ScrollHandDrag);
    tmpSrc = NULL;
}

/** Destructor */
GraphView::~GraphView()
{

}


void 
GraphView::drawBackground(QPainter *painter, const QRectF &rect)
{

}

void 
GraphView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        QPoint p = ev->pos();
        if ( !scene()->itemAt( mapToScene( ev->pos())))
        {
            GNode* node = graph()->newNode();
            node->item()->setPos( mapToScene( p));
        }
    } else if( ev->button() & Qt::RightButton)
    {
        QGraphicsItem *node = scene()->itemAt( mapToScene( ev->pos()));
        if ( isNotNullP( node) && qgraphicsitem_cast<NodeItem *>( node))
        {
            delete qgraphicsitem_cast<NodeItem *>( node)->node();
        }
    }
    QGraphicsView::mouseDoubleClickEvent( ev);   
}

void
GraphView::mousePressEvent(QMouseEvent *ev)
{
    QGraphicsView::mousePressEvent( ev);
}

void
GraphView::mouseReleaseEvent(QMouseEvent *ev)
{
    if( ev->button() & Qt::RightButton)
    {
        if ( createEdge)
        {
            QGraphicsItem* item = scene()->itemAt( mapToScene( ev->pos()));
            if ( isNotNullP( item) && qgraphicsitem_cast<NodeItem *>(item))
            {
                if ( tmpSrc != qgraphicsitem_cast<NodeItem *>(item)->node())
                {
                    graph()->newEdge( tmpSrc, qgraphicsitem_cast<NodeItem *>(item)->node());
                }
            }
        }
    }
    tmpSrc = NULL;
    createEdge = false;
    QGraphicsView::mouseReleaseEvent(ev);
}

void
GraphView::mouseMoveEvent(QMouseEvent *ev)
{
    if ( createEdge)
    {
        dst = ev->pos();
    }
    QGraphicsView::mouseMoveEvent(ev);
}

/**
 * Actions for wheel event
 */
void GraphView::wheelEvent(QWheelEvent *event)
{
    zoom_scale += event->delta() / 100;
	updateMatrix();
}

void GraphView::zoomIn()
{
    zoom_scale++;
    updateMatrix();
}

void GraphView::zoomOut()
{
	zoom_scale--;
    updateMatrix();
}

void GraphView::zoomOrig()
{
    zoom_scale = 0;
    ensureVisible( 0,0,0,0);
    checkDelItems();
    updateMatrix();
}

void GraphView::updateMatrix()
{
     qreal scale_val = qPow( qreal(2), zoom_scale / qreal(5)); 
     QMatrix matrix;
     matrix.scale(scale_val, scale_val);
   
     setMatrix(matrix);
}

void
GraphView::deleteItems()
{
    int depth = scene()->bspTreeDepth();
    scene()->setBspTreeDepth( 1);
    scene()->setItemIndexMethod( QGraphicsScene::NoIndex);
    foreach ( NodeItem* item, del_node_items)
    {
        del_node_items.removeAll( item);  
        delete item;
    }
    foreach ( EdgeItem* item, del_edge_items)
    {
        del_edge_items.removeAll( item);  
        delete item;  
    }
    scene()->setItemIndexMethod( QGraphicsScene::BspTreeIndex);
    scene()->setBspTreeDepth( depth);
}
