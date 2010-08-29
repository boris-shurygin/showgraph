/**
 * @file: graph_view.cpp
 * Graph View Widget implementation.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009 Boris Shurygin
 */
#include "gview_impl.h"

inline qreal scaleVal( qreal zoom_scale)
{
    return qPow( qreal(2), zoom_scale / qreal(5));
}

/** Destructor */
GGraph::~GGraph()
{
    freeMarker( nodeTextIsShown);
    for ( GNode *node = firstNode();
          isNotNullP( node);
          )
    {
        GNode* next = node->nextNode();
        int ir_id = node->irId();
        deleteNode( node);
        node = next;
    }
}

GNode*
GGraph::newNode()
{
    GNode* n = static_cast< GNode *>( AuxGraph::newNode());
    return n;
}

GEdge*
GGraph::newEdge( GNode* pred, GNode* succ)
{
    GEdge* e = 
        static_cast< GEdge *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ));
    return e;
}

GNode*
GGraph::newNode( QDomElement e)
{
    GNode* n =  static_cast< GNode *>( AuxGraph::newNode( e));
    return n;
}

GEdge*
GGraph::newEdge( GNode* pred, GNode* succ, QDomElement e)
{
    GEdge* edge_p = 
        static_cast< GEdge *>( AuxGraph::newEdge( (AuxNode *)pred, (AuxNode *)succ, e));
    return edge_p;
}

void GGraph::selectOneNode( GNode* n)
{    
    emptySelection();
    selectNode( n);
    n->item()->highlight();
    n->item()->update();
}

void GGraph::showNodesText()
{
    foreach (GNode *n, sel_nodes)
    {
        view()->showNodeText( n);
    }
}

/**
 * Delete scheduled nodes
 */
void GGraph::deleteNodes()
{
    foreach (GNode *n, sel_nodes)
    {
        deleteNode( n);
    }
    sel_nodes.clear();
}

/**
 * Delete scheduled edges
 */
void GGraph::deleteEdges()
{
	foreach (GEdge *e, sel_edges)
	{
		deleteEdgeWithControls( e);
	}
    sel_edges.clear();
}
/**
 * Create self edge on selected node
 */
void GGraph::createSelfEdge()
{
    if ( !sel_nodes.isEmpty())
    {
        GNode *n = sel_nodes.first();
        newEdge( n, n);
    }
}

/** Create label on selected edge */
void GGraph::createEdgeLabel( QPointF pos)
{
    if ( !sel_edges.isEmpty())
    {
        sel_edges.first()->insertLabelNode( pos);
    }
}

void GGraph::showEdgePred()
{
    if ( !sel_edges.isEmpty())
    {
        if ( view()->isContext())
        {
            selectNode( sel_edges.first()->realPred());
            view()->findContext();
        } else
        {
            view()->focusOnNode( sel_edges.first()->realPred(), true);
        }
    }
}

void GGraph::showEdgeSucc()
{
    if ( !sel_edges.isEmpty())
    {
        if ( view()->isContext())
        {
            selectNode( sel_edges.first()->realSucc());
            view()->findContext();
        } else
        {
            view()->focusOnNode( sel_edges.first()->realSucc(), true);
        }
    }
}

void GGraph::clearNodesPriority()
{
    GNode* n;
    foreachNode( n, this)
    {
        n->setPriority( 0);
    }
}

void GGraph::showWholeGraph()
{
    assert( !view()->isContext());
    emptySelection();
    GNode* n;
    foreachNode( n, this)
    {
        n->item()->setVisible( true);
        n->item()->setOpacityLevel( 1);
        n->setPriority( MAX_OPACITY);
        n->setForPlacement( true);
        n->setStable( false);
        n->item()->update();
    }
    doLayout();
}

/** Create label on selected edge */
void GGraph::findContext()
{
    if ( sel_nodes.isEmpty())
        return;
    
    QQueue< GNode *> border;
    Marker m = newMarker();
    foreach( GNode *n, sel_nodes)
    {
        n->mark( m);
        n->setPriority( MAX_PRIORITY);
        //n->setStable( true);
        border.enqueue( n);
    }
    for ( int i = 0; i < MAX_PLACE_LEN; i++)
    {
        int added = border.count();
        for ( int j = 0; j < added; j++)
        {
            GNode *n = border.dequeue(); 
            if ( !n->isPseudo())
            {
                selectNode( n);
                n->item()->highlight();
            }
            n->item()->update();
            GEdge *e;
            foreachPred( e, n)
            {
                GNode * pred = e->pred();
                
                if ( pred->mark( m))
                {
                    if ( i <= MAX_VISIBLE_LEN 
                         && pred->priority() < MAX_VISIBLE_LEN - i)
                    {
                        pred->setPriority( MAX_VISIBLE_LEN - i);
                    }/* else
                    {
                        pred->setPriority( 0);
                    }*/
                    border.enqueue( pred);
                }
            }
            foreachSucc( e, n)
            {
                GNode * succ = e->succ();
                if ( succ->mark( m))
                {
                    if ( i <= MAX_VISIBLE_LEN
                         && succ->priority() < MAX_VISIBLE_LEN - i)
                    {
                        succ->setPriority( MAX_VISIBLE_LEN - i);
                    }/* else
                    {
                        succ->setPriority( 0);
                    }*/
                    border.enqueue( succ);
                }
            }
        }
    }
    if ( view()->isContext())
    {
        GNode* n;
        foreachNode( n, this)
        {
            /*if ( n->item()->isVisible())
            {
                n->setStable();
            }*/
            if ( n->isMarked( m))
            {
                if ( !n->item()->isVisible())
                {
                    n->item()->setVisible( true);
                    n->item()->setOpacityLevel( 0);
                }
                n->setForPlacement( true);
            } else
            {
                if ( n->priority() < MAX_PRIORITY)
                {
                    n->setPriority( 0);
                    n->setForPlacement( false);
                }
                n->setStable( false);
            }
        }
        GEdge *e;
        foreachEdge( e, this)
        {
            if ( e->pred()->isPseudo() && e->succ()->isPseudo())
            {
                if ( e->realPred()->item()->isVisible()
                     && e->realSucc()->item()->isVisible())
                {
                    int priority = 
                        min<int>( e->realPred()->priority(), e->realSucc()->priority());
                    e->pred()->item()->setVisible( true);   
                    e->succ()->item()->setVisible( true);   
                    e->pred()->setForPlacement( true);
                    e->succ()->setForPlacement( true);
                    e->pred()->setPriority( priority);
                    e->succ()->setPriority( priority);
                }
            }
        }
        selectOneNode( sel_nodes.first());
    
        if ( view()->hasSmoothFocus())
        {
            view()->focusOnNode( sel_nodes.first(), true);
        }
        doLayout(); 
        foreachNode( n, this)
        {
            if ( n->item()->isVisible() && !n->item()->opacityLevel())
            {
                n->item()->setPos( n->modelX(), n->modelY());
            }
        }
        view()->startAnimationNodes();
    }
    freeMarker( m);
    
}

void GGraph::deleteEdgeWithControls( GEdge *edge)
{
	QList< GNode *> nodes;
	QList< GEdge *> edges;
    
    /** Check successor */
    GNode * succ = edge->succ();
    while ( succ->isEdgeControl() || succ->isEdgeLabel())
    {
        assert( isNotNullP( succ->firstSucc()));
        nodes << succ;
        edges << succ->firstSucc();
		succ = succ->firstSucc()->succ();
    }
    GNode * pred = edge->pred(); 
    while ( pred->isEdgeControl() || pred->isEdgeLabel())
    {
        assert( isNotNullP( pred->firstPred()));
        nodes << pred;
		edges << pred->firstPred();
        pred = pred->firstPred()->pred();
    }
    deleteEdge( edge);
	foreach ( GEdge *e, edges)
    {
        deleteEdge( e);
    }
    foreach ( GNode *n, nodes)
    {
        deleteNode( n);
    }
	
}

void GGraph::UpdatePlacement()
{
	QGraphicsScene *scene = view()->scene();
	
	/**
	 * Prevent BSP tree from being modified - significantly speeds up node positioning 
	 * For this purpose tree depth is set to one and indexing is turned off
	 */
	int depth = scene->bspTreeDepth();
    scene->setBspTreeDepth( 1);
    scene->setItemIndexMethod( QGraphicsScene::NoIndex);
    GNode *n;
	for ( n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        n->item()->setPos( n->modelX(), n->modelY());
    }

    GEdge *e;
    
    foreachNode( n, this)
    {
        n->item()->show();
    }
    foreachEdge( e, this)
    {
        e->item()->show();
    }
	/** Restore indexing */
	scene->setItemIndexMethod( QGraphicsScene::BspTreeIndex);
    scene->setBspTreeDepth( depth); 
}
/**
 * Run layout procedure
 */
void GGraph::doLayout()
{
    if ( view()->isContext())
    {
        if ( rankingValid())
        {
            arrangeHorizontally();
        } else
        {
            /** Run layout algorithm */
	        AuxGraph::doLayout();
            UpdatePlacement();
        }
    } else
    {
        GNode *n;
        GEdge *e;
        
        foreachNode( n, this)
        {
            n->item()->hide();
        }
        foreachEdge( e, this)
        {
            e->item()->hide();
        }
        /** Run layout algorithm */
	    AuxGraph::doLayoutConcurrent();
        //AuxGraph::doLayout();
        //layoutPostProcess();
    }
}

/**
 * Run layout procedure in single thread
 */
void GGraph::doLayoutSingle()
{
    if ( view()->isContext())
    {
        if ( rankingValid())
        {
            arrangeHorizontally();
        } else
        {
            /** Run layout algorithm */
	        AuxGraph::doLayout();
            UpdatePlacement();
        }
    } else
    {
        GNode *n;
        GEdge *e;
        
        foreachNode( n, this)
        {
            n->item()->hide();
        }
        foreachEdge( e, this)
        {
            e->item()->hide();
        }
        /** Run layout algorithm */
	    AuxGraph::doLayout();
        layoutPostProcess();
    }
}

void GGraph::layoutPostProcess()
{
    UpdatePlacement();
    
    /** Center view on root node */
    GNode *root = static_cast<GNode*>( rootNode());
    if ( isNotNullP( root))
    {
        view_p->centerOn( root->item());
    }
}

/** Constructor */
GraphView::GraphView(): 
    curr_pos(),
    createEdge( false),
    graph_p( 0),
	zoom_scale( 0),
    view_history( new GraphViewHistory),
    timer_id( 0),
    node_animation_timer( 0),
    smooth_focus( false),
    editable( false),
    view_mode( WHOLE_GRAPH_VIEW)
{
    QGraphicsScene *scene = new QGraphicsScene( this);
    //scene->setItemIndexMethod( QGraphicsScene::NoIndex);
    //scene->setSceneRect(0, 0, 10000, 10000);
    setScene( scene);
    //setCacheMode( CacheBackground);
    setViewportUpdateMode( SmartViewportUpdate);
    setRenderHint( QPainter::Antialiasing);
    setTransformationAnchor( AnchorViewCenter);
    setResizeAnchor( AnchorViewCenter);
    setMinimumSize( 200, 200);
    setWindowTitle( tr("ShowGraph"));
    setDragMode( ScrollHandDrag);
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
    scene()->setItemIndexMethod( QGraphicsScene::NoIndex);
    delete graph_p;
    delete view_history;
}

void GraphView::startAnimationNodes()
{
    if ( !node_animation_timer)
         node_animation_timer = startTimer(1000/25);
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
        if ( isEditable() && !scene()->itemAt( mapToScene( ev->pos())))
        {
            GNode* node = graph()->newNode();
            node->item()->setPos( mapToScene( p));
        }
    } else if( isEditable() 
               && ev->button() & Qt::RightButton)
    {
        QGraphicsItem *node = scene()->itemAt( mapToScene( ev->pos()));
        if ( isNotNullP( node) && qgraphicsitem_cast<NodeItem *>( node))
        {
            graph()->emptySelection();
            graph()->setNodeInFocus( NULL);
            graph()->deleteNode( qgraphicsitem_cast<NodeItem *>( node)->node());
        }
    }
    QGraphicsView::mouseDoubleClickEvent( ev);   
}

void
GraphView::mousePressEvent(QMouseEvent *ev)
{
    if( timer_id)
    {
        killTimer( timer_id);
        timer_id = 0;
    }
    QGraphicsView::mousePressEvent( ev);
}

/** Enable/disable edition */
void GraphView::toggleEdition( bool e)
{
    setEditable( e);
}

/** Insert node in center of view */
void GraphView::insertNodeOnCenter()
{

}

/** Run layout */
void GraphView::runLayout()
{
    graph()->doLayout();
}

void
GraphView::contextMenuEvent( QContextMenuEvent * e)
{

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
                    GNode* dst_node = qgraphicsitem_cast<NodeItem *>(item)->node();
                    if ( tmpSrc->isSimple() && dst_node->isSimple())
                    {   
                        graph()->newEdge( tmpSrc, dst_node);
                        show_menus = false;
                    }
				}
            }
			
        } else if ( !scene()->itemAt( mapToScene( ev->pos())))
        {
            QMenu *menu = new QMenu( tr( "&View Menu"));
            menu->addAction( editableSwitchAct);
            menu->addAction( runLayoutAct);
            menu->exec( mapToGlobal( ev->pos()));
            delete menu;
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
    QGraphicsView::mouseMoveEvent(ev);
}

void 
GraphView::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Plus)
    {
        zoomIn();
    } else if ( event->key() == Qt::Key_Minus)
    {
        zoomOut();
    }
    QGraphicsView::keyPressEvent( event);
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
     qreal scale_val = scaleVal( zoom_scale); 
     QMatrix scale;
     qreal prev_scale = matrix().m11();
     qreal scale_ratio = scale_val / prev_scale; 
     scale.scale( scale_val, scale_val);
     GNode * focus = graph()->nodeInFocus();
     if ( isNotNullP( focus))
     {
         QPointF item_center = focus->item()->mapToScene( focus->item()->boundingRect())
                               .boundingRect()
                               .center();
         QPointF old_center = mapToScene( viewport()->rect())
                             .boundingRect()
                             .center();

         QPointF item_in_view = item_center - old_center;
         setMatrix( scale); // scale
         QPointF new_center = old_center + item_in_view * ( scale_ratio - 1) / scale_ratio;
         centerOn( new_center); // adjust to keep focus point in place
     } else
     {
         setMatrix( scale);
     }
     
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

void GraphView::createEdgeLabel()
{
    graph()->createEdgeLabel( curr_pos);
}

void GraphView::showEdgePred()
{
    graph()->showEdgePred();
}

void GraphView::showEdgeSucc()
{
    graph()->showEdgeSucc();
}

void GraphView::findContext()
{
    graph()->findContext();
}

/** Show text of the clicked node */
void GraphView::showSelectedNodesText()
{
    graph()->showNodesText();
}

void GraphView::createActions()
{
    QString system = QLatin1String("win");
#ifdef Q_OS_MAC
    system = QLatin1String("mac");
#endif

    deleteItemAct = new QAction(tr("&Delete"), this);
    deleteItemAct->setShortcut(tr("Ctrl+D"));
    connect(deleteItemAct, SIGNAL(triggered()), this, SLOT( deleteSelected()));

    createSelfEdgeAct = new QAction(tr("&Create self-edge"), this);
    connect( createSelfEdgeAct, SIGNAL(triggered()), this, SLOT( createSESelected()));

    createEdgeLabelAct = new QAction(tr("&Create label"), this);
    connect( createEdgeLabelAct, SIGNAL(triggered()), this, SLOT( createEdgeLabel()));

    findContextAct = new QAction(tr("&Highlight context"), this);
    connect( findContextAct, SIGNAL(triggered()), this, SLOT( findContext()));
    
    showTextAct = new QAction(tr("&Show text"), this);
    connect( showTextAct, SIGNAL(triggered()), this, SLOT( showSelectedNodesText()));

    showPredAct = new QAction(tr("Pred"), this);
    connect( showPredAct, SIGNAL(triggered()), this, SLOT( showEdgePred()));

    showSuccAct = new QAction(tr("Succ"), this);
    connect( showSuccAct, SIGNAL(triggered()), this, SLOT( showEdgeSucc()));

    editableSwitchAct = 
        new QAction( QIcon( QString::fromUtf8("images/%1/Edit/Edit.ico").arg( system)),
                     tr("Editable"), this);
    editableSwitchAct->setCheckable( true);
    editableSwitchAct->setChecked( false);
    connect( editableSwitchAct, SIGNAL( toggled( bool)), this, SLOT( toggleEdition( bool)));
    
    insertNodeAct = new QAction(tr("Insert node"), this);
   
    runLayoutAct = 
        new QAction( QIcon( QString::fromUtf8("images/%1/Synchronize/Synchronize.ico").arg( system)),
                     tr("&Run Layout"), this);
    connect( runLayoutAct, SIGNAL(triggered()), this, SLOT( runLayout()));
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
    QMenu* menu = new QMenu( tr( "&Node"));
    menu->addAction( deleteItemAct);
    deleteItemAct->setEnabled( isEditable());
    if ( !n->isPseudo())
    {
        menu->addAction( findContextAct);
        menu->addAction( showTextAct);
        menu->addSeparator();
        menu->addAction( createSelfEdgeAct);
        createSelfEdgeAct->setEnabled( isEditable());
    }
    return menu;
}

QMenu* GraphView::createMenuForEdge( GEdge *e)
{
    QMenu* menu = new QMenu( tr( "&Edge"));
    menu->addAction( showPredAct);
    menu->addAction( showSuccAct);
    menu->addSeparator();
    menu->addAction( deleteItemAct);
    deleteItemAct->setEnabled( isEditable());
    menu->addAction( createEdgeLabelAct);
    createEdgeLabelAct->setEnabled( isEditable());
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

void GraphView::advanceNodes()
{
    GNode *n;
    bool advanced = false;
    foreachNode( n, graph())
    {
        if ( n->item()->advance())
            advanced = true;
    }
    if ( !advanced)
    {
        killTimer( node_animation_timer);
        node_animation_timer = 0;
    }
}

void GraphView::advanceView()
{
    GNode *target = graph()->nodeInFocus(); 
    qreal STEP_LEN = 10 / scaleVal( zoom_scale);
    const qreal STEP_SCALE = 0.2;
    if ( isNotNullP( target))
    {
        QRectF item_rect = target->item()->mapToScene( target->item()->boundingRect()).boundingRect();
        QRectF view_rect = mapToScene( viewport()->rect())
                           .boundingRect();
        
        QLineF line( view_rect.center(), item_rect.center());
        
        qreal dist = line.length();
        QPointF displacement( STEP_LEN * line.dx() / dist, STEP_LEN * line.dy() / dist);
        if ( dist > STEP_LEN /2)
            centerOn( view_rect.center() + displacement);
        
        QRectF new_view_rect = mapToScene( viewport()->rect()).boundingRect();
        QLineF step( new_view_rect.center(), view_rect.center());
        qreal len = step.length();
        
        if ( zoom_out_done)
        {   
            if ( dist < STEP_LEN || len < STEP_LEN /2)
            {
                zoom_scale+=STEP_SCALE;
                updateMatrix();
                if ( abs<qreal>( zoom_scale - preferred_zoom) <= STEP_SCALE * 2)
                {
                    zoom_scale = preferred_zoom;
                    updateMatrix();
                    killTimer( timer_id);
                    timer_id = 0;
                    centerOn( target->item());
                }
            }
        }
        if ( ! zoom_out_done
             && ( abs<qreal>( line.dx()) > view_rect.width()
                  || abs<qreal>( line.dy()) > view_rect.height()))
        {
            zoom_scale-=STEP_SCALE;
            QMatrix scale;
            scale.scale( scaleVal( zoom_scale), scaleVal( zoom_scale));
            setMatrix( scale);
        } else
        {
            zoom_out_done = true;
        }
    }
}

void GraphView::timerEvent( QTimerEvent *event)
{
    if ( !event->timerId())
        return;
    if ( event->timerId() == timer_id)
    {
        advanceView();
    }
    if ( event->timerId() == node_animation_timer)
    {
        advanceNodes();
    }
}
void GraphView::clearSearch()
{
    search_node = NULL;
}

void GraphView::focusOnNode( GNode *n, bool gen_event)
{
    graph()->selectOneNode( n);
    
    if ( smooth_focus)
    {
        if (!timer_id)
             timer_id = startTimer(1000/25);
        preferred_zoom = zoom_scale;
        zoom_out_done = false;
    } else
    {
        centerOn( n->item());
    }
    if ( gen_event && !n->isNodeInFocus())
        view_history->focusEvent( n);
    graph()->setNodeInFocus( n);
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
		if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
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
		if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
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
GNode* GraphView::findNodeById( int id)
{
	GNode *n;
	foreachNode( n, graph())
	{
        if ( !n->isEdgeControl()
             && !n->isEdgeLabel()
             && n->irId() == id)
			break;
	}
	if ( isNotNullP( n))
	{
        if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( n);
            findContext();
        }
        focusOnNode( n, true);
		return n;
	} else
	{
		return NULL;
	}
}
/** Repeat navigation event */
void GraphView::replayNavigationEvent( NavEvent *ev)
{
    assert( isNotNullP( ev));
    
    if ( ev->isFocus())
    {
        assert( isNotNullP( ev->node()));
        if ( isContext())
        {
            graph()->emptySelection();
            graph()->selectNode( ev->node());
            findContext();
        }
        focusOnNode( ev->node(), false); 
    }
}

void GraphView::toggleSmoothFocus( bool smooth)
{
    smooth_focus = smooth;
}

void GraphView::toggleViewMode( bool context)
{
    if ( context && CONTEXT_VIEW != view_mode)
    {
        view_mode = CONTEXT_VIEW;
        graph()->clearNodesPriority();
    } else if ( !context && WHOLE_GRAPH_VIEW != view_mode)
    {
        view_mode = WHOLE_GRAPH_VIEW;
        graph()->showWholeGraph();
    }
}

/** Navigate backward */
void GraphView::navPrev()
{
    NavEvent * ev = view_history->prev();
    if ( isNotNullP( ev))
        replayNavigationEvent( ev);
}

/** Navigate forward */
void GraphView::navNext()
{
    NavEvent * ev = view_history->next();
    if ( isNotNullP( ev))
        replayNavigationEvent( ev);
}
/** Erase node from history */
void GraphViewHistory::eraseNode( GNode *n)
{
    it = events.begin();
    while ( it != events.end())
    {
        NavEvent *ev = *it;
        if ( ev->isFocus() && areEqP( ev->node(), n))
        {
            it = events.erase( it);
        } else
        {
            it++;
        }
    }
}
/** Get last event */
NavEvent *GraphViewHistory::last()
{
    if ( events.count() > 0)
    {
        it = events.end();
        it--;
        return *it;
    }
    return NULL;
}
/** Get prev event */
NavEvent *GraphViewHistory::prev()
{
    if ( it != events.begin())
    {
        it--;
        return *it;
    }
    return NULL;
}
/** Get next event */
NavEvent *GraphViewHistory::next()
{
    if ( it != events.end())
        it++;
    if ( it != events.end())
    {
        return *it;
    }
    return NULL;
}
/** New event */
void GraphViewHistory::viewEvent( NavEventType t, GNode *n)
{
    NavEvent *ev = new NavEvent( t, n);
    if ( it != events.end())
        it++;
    if ( it != events.end())
        events.erase( it, events.end());
    events.push_back( ev);
    it = events.end();
    it--;
}
