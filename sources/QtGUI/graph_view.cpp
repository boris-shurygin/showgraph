/**
 * @file: graph_view.cpp
 * Graph View Widget implementation.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009 Boris Shurygin
 */
#include "gui_impl.h"

/** Destructor */
GGraph::~GGraph()
{
    freeMarker( nodeTextIsShown);
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

void GGraph::deleteEdgeWithControls( GEdge *edge)
{
	QList< GNode *> nodes;
	QList< GEdge *> edges;
    
    /** Check successor */
    GNode * succ = edge->succ();
	while ( succ->isEdgeControl())
    {
        assert( isNotNullP( succ->firstSucc()));
        nodes << succ;
        edges << succ->firstSucc();
		succ = succ->firstSucc()->succ();
    }
    GNode * pred = edge->pred(); 
    while ( pred->isEdgeControl())
    {
        assert( isNotNullP( pred->firstPred()));
        nodes << pred;
		edges << pred->firstPred();
        pred = pred->firstPred()->pred();
    }
    delete edge;
	foreach ( GEdge *e, edges)
    {
        delete e;
    }
    foreach ( GNode *n, nodes)
    {
        delete n;
    }
	
}

/**
 * Run layout procedure
 */
void GGraph::doLayout()
{
    /** Run layout algorithm */
	AuxGraph::doLayout();
    
	QGraphicsScene *scene = view()->scene();
	
	/**
	 * Prevent BSP tree from being modified - significantly speeds up node positioning 
	 * For this purpose tree depth is set to one and indexing is turned off
	 */
	int depth = scene->bspTreeDepth();
    scene->setBspTreeDepth( 1);
    scene->setItemIndexMethod( QGraphicsScene::NoIndex);

	for ( GNode* n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        n->item()->setPos( n->modelX(), n->modelY());
    }
    /** Center view on root node */
	GNode *root = static_cast<GNode*>( rootNode());
    if ( isNotNullP( root))
    {
        view_p->centerOn( root->item());
    }
	/** Restore indexing */
	scene->setItemIndexMethod( QGraphicsScene::BspTreeIndex);
    scene->setBspTreeDepth( depth);
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
    //setDragMode( RubberBandDrag);
    tmpSrc = NULL;
    search_node = NULL;
	createActions();
	createMenus();
	show_menus = true;
	setAcceptDrops( false);
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
GraphView::mouseReleaseEvent( QMouseEvent *ev)
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
					show_menus = false;
				}
            }
			
        }
    }
    QGraphicsView::mouseReleaseEvent(ev);
	createEdge = false;
	show_menus = true;
	tmpSrc = NULL;
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

void GraphView::deleteSelected()
{
    graph()->deleteNodes();
	graph()->deleteEdges();
}

void GraphView::createSESelected()
{
    graph()->createSelfEdge();
}

void GraphView::createActions()
{
    deleteItemAct = new QAction(tr("&Delete"), this);
    deleteItemAct->setShortcut(tr("Ctrl+D"));
    connect(deleteItemAct, SIGNAL(triggered()), this, SLOT( deleteSelected()));

    createSelfEdgeAct = new QAction(tr("&Create Self Edge"), this);
    connect( createSelfEdgeAct, SIGNAL(triggered()), this, SLOT( createSESelected()));
}

void GraphView::createMenus()
{
    nodeItemMenu = new QMenu( tr( "&Node Item"));
    nodeItemMenu->addAction( deleteItemAct);
    nodeItemMenu->addAction( createSelfEdgeAct);

	edgeItemMenu = new QMenu( tr( "&Edge Item"));
    edgeItemMenu->addAction( deleteItemAct);
}

QMenu* GraphView::createMenuForNode( GNode *n)
{
    QMenu* menu = new QMenu( tr( "&Node Item"));
    menu->addAction( deleteItemAct);
    if ( !n->isEdgeControl())
    {
        menu->addAction( createSelfEdgeAct);
    }
    return menu;
}

void GraphView::dragEnterEvent(QDragEnterEvent *event)
{
	/*if ( event->mimeData()->hasUrls())
		event->acceptProposedAction();*/
}

void GraphView::dropEvent(QDropEvent *event)
{
	/*const QMimeData *mimeData = event->mimeData();

	if ( mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		QString text;

		for ( int i = 0; i < urlList.size(); ++i)
		{
			QString url = urlList.at(i).path();

		}
    }

	event->acceptProposedAction();*/
}
 
void GraphView::dragMoveEvent( QDragMoveEvent *event)
{
	//event->acceptProposedAction();
}

void GraphView::clearSearch()
{
    search_node = NULL;
}

GNode *
GraphView::findNextNodeWithText( QString &findStr,
                                 QTextDocument::FindFlags flags)
{
	GNode *n;
	
    if ( isNullP( search_node))
    {
        foreachNode( n, graph())
        {
            if ( isNullP( n->nextNode()))
                break;
        }
    } else
    {
        n = search_node->prevNode();
    }

    while ( isNotNullP( n))
    {
		QTextDocument *doc = n->doc();
		if ( isNotNullP( doc) && !doc->find( findStr, 0, flags).isNull())
        {
            search_node = n;        
            break;
        }
        n = n->prevNode();
	}
	if ( isNotNullP( n))
	{
		centerOn( n->item());
		return n;
	} else
	{
		search_node = NULL;        
        return NULL;
	}
}

GNode *
GraphView::findPrevNodeWithText( QString &findStr,
                                 QTextDocument::FindFlags flags)
{
	GNode *n;
	
    flags ^= QTextDocument::FindBackward; // Unset backward search flag
    for ( n = isNullP( search_node) ? graph()->firstNode() : search_node->nextNode();
          isNotNullP( n);
          n = n->nextNode())
    {
		QTextDocument *doc = n->doc();
		if ( isNotNullP( doc) && !doc->find( findStr, 0, flags).isNull())
        {
            search_node = n;        
            break;
        }
	}
	if ( isNotNullP( n))
	{
		centerOn( n->item());
		return n;
	} else
	{
		search_node = NULL;        
        return NULL;
	}
}
/**
 * Find node by its ID from dump
 */
bool GraphView::findNodeById( int id)
{
	GNode *n;
	foreachNode( n, graph())
	{
		if ( n->irId() == id)
			break;
	}
	if ( isNotNullP( n))
	{
		centerOn( n->item());
		return true;
	} else
	{
		return false;
	}
}