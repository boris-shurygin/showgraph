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

    ranking = newNum();
    ordering = newNum();
}

/** Destructor */
GraphView::~GraphView()
{
    freeNum( ranking);
    freeNum( ordering);
}

void 
GraphView::drawBackground(QPainter *painter, const QRectF &rect)
{

}

NodeItem*
GraphView::newNode()
{
    NodeItem* n = GraphT< GraphView, NodeItem, EdgeItem>::newNode();
    scene()->addItem( n);
    return n;
}

EdgeItem*
GraphView::newEdge( NodeItem* pred, NodeItem* succ)
{
    EdgeItem* e = GraphT< GraphView, NodeItem, EdgeItem>::newEdge( pred, succ);
    scene()->addItem( e);
    e->initControls();
    return e;
}

NodeItem*
GraphView::newNode( QDomElement e)
{
    NodeItem* n = GraphT< GraphView, NodeItem, EdgeItem>::newNode( e);
    scene()->addItem( n);
    return n;
}

EdgeItem*
GraphView::newEdge( NodeItem* pred, NodeItem* succ, QDomElement e)
{
    EdgeItem* edge_p = GraphT< GraphView, NodeItem, EdgeItem>::newEdge( pred, succ, e);
    scene()->addItem( edge_p);
    edge_p->initControls();
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

/**
 * Implementation of layout-oriented part of graph
 */

/**
 * Perform edge classification
 */
void GraphView::classifyEdges()
{
    /** Structure used for dfs traversal */
    struct DfsStepInfo
    {
        NodeItem *node; // Node in consideration
        EdgeItem *edge; // Next edge

        /* Constructor */
        DfsStepInfo( NodeItem *n)
        {
            node = n;
            edge = n->firstSucc();
        }
    };

    Marker m = newMarker(); // Marker for visiting nodes
    Marker doneMarker = newMarker(); // Marker for nodes that are finished
    QStack< DfsStepInfo *> stack;
    
    /* Fill stack with nodes that have no predecessors */
    for ( NodeItem *n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        if ( isNullP( n->firstPred()))
        {
            stack.push( new DfsStepInfo( n));
        }
    }
    /* Walk graph with marker and perform classification */
    while( !stack.isEmpty())
    {
        DfsStepInfo *info = stack.top();
        NodeItem *node = info->node;
        EdgeItem *edge = info->edge;
        
        if ( isNotNullP( edge)) // Add successor to stack
        {
            NodeItem* succ_node = edge->succ();
            info->edge = edge->nextSucc();
            
            if ( succ_node->isMarked( doneMarker))
            {
                assert( succ_node->isMarked( m));
                edge->setBack();
            }
            if ( succ_node->mark( m))
                 stack.push( new DfsStepInfo( succ_node));
        } else // We're done with this node
        {
            node->mark( doneMarker);
            delete info;
            stack.pop();
        }
    }

    freeMarker( m);
    freeMarker( doneMarker);
    return;
}

/**
 * Ranking of nodes. Level distribution of nodes. Marks tree edges.
 */
Numeration GraphView::rankNodes()
{
    QVector< int> pred_nums( nodeCount());
    classifyEdges();
    Numeration own = newNum();
    int i = 0;

    /* Set numbers to nodes */
    for ( NodeItem *n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        n->setNumber( own, i);
        i++;
    }

    freeNum( own);
    return ranking; 
}