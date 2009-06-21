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
            
            if ( !succ_node->isMarked( doneMarker)
                 && succ_node->isMarked( m))
            {
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
    QStack< NodeItem *> stack; // Node stack
    
    Numeration own = newNum();
    GraphNum i = 0;
    max_rank = 0;
    /**
     *  Set numbers to nodes and count predecessors of each node.
     *  predecessors include inverted edges 
     */
    for ( NodeItem *n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        int pred_num = 0;
        n->setNumber( own, i);
        for ( EdgeItem* e = n->firstPred();
              isNotNullP( e);
              e = e->nextPred())
        {
            if ( !e->isInverted())
                pred_num++;
        }
        for ( EdgeItem* e = n->firstSucc();
              isNotNullP( e);
              e = e->nextSucc())
        {
            if ( e->isInverted())
                pred_num++;
        }
        pred_nums[ i] = pred_num;
        i++;
    }
    /* Fill ranking and ordering numerations by walking the nodes */
    /* Add nodes with no preds to stack */
    for ( NodeItem *n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        if ( pred_nums[ n->number( own)] == 0)
        {
            stack.push( n);
        }
    }
    while( !stack.isEmpty())
    {
        NodeItem* n = stack.pop();
        GraphNum rank = 0;

        /* Propagation part */
        for ( EdgeItem* e = n->firstPred();
              isNotNullP( e);
              e = e->nextPred())
        {
            if ( !e->isInverted())
            {
                if ( rank < e->pred()->number( ranking) + 1)
                {
                    rank = e->pred()->number( ranking) + 1;
                }
            }
        }
        for ( EdgeItem* e = n->firstSucc();
              isNotNullP( e);
              e = e->nextSucc())
        {
            if ( e->isInverted())
            {
                if ( rank < e->succ()->number( ranking) + 1)
                {
                    rank = e->succ()->number( ranking) + 1;
                }
            }
        }

        if ( rank > max_rank)
            max_rank = rank;

        n->setNumber( ranking, rank);
        /* FIXME: just for debugging */
        n->setPos( n->pos().x(), rank * RANK_SPACING);

        /* Traversal continuation */
        for ( EdgeItem* e = n->firstSucc();
              isNotNullP( e);
              e = e->nextSucc())
        {
            if ( !e->isInverted())
            {
                NodeItem* succ = e->succ();
                pred_nums[ succ->number( own)] =
                    pred_nums[ succ->number( own)] - 1;
                
                if ( pred_nums[ succ->number( own)] == 0)
                {
                    stack.push( succ);
                }
            }  
        }
        for ( EdgeItem* e = n->firstPred();
              isNotNullP( e);
              e = e->nextPred())
        {
            if ( e->isInverted())
            {
                NodeItem* succ = e->pred();
                pred_nums[ succ->number( own)] =
                    pred_nums[ succ->number( own)] - 1;
                if ( pred_nums[ succ->number( own)] == 0)
                {
                    stack.push( succ);
                }
            }
        }
    }
    freeNum( own);
    return ranking; 
}

/**
 * Perform layout
 */
void GraphView::doLayout()
{
    /** 1. Perfrom edge classification */
    classifyEdges();
    
    /** 2. Rank nodes */
    rankNodes();

    /** 3. Build aux graph */
    AuxGraph* agraph = new AuxGraph( this);

    /** 4. Perform edge crossings minimization */
    agraph->reduceCrossings();

    /** 5. Perform horizontal arrangement of nodes */
    agraph->arrangeHorizontally();

    /** 6. Move edge controls to enchance the picture readability */

    /** Delete temporary structures */
    delete agraph;
}