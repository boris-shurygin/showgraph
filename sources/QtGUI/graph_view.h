/**
 * @file: graph_view.h 
 * Graph View class definition.
 * @defgroup GUIGraph Graph Visualization System
 * @ingroup GUI
 */
/* GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
/**
 * @page iface Graph editor interface
 * Graph editor.
 * 
 * Here's list of conventional editing tasks and how they are performed with Showgraph
 * -# Create new node - double click on free space
 * -# Create edge - draw edge holding down right mouse button
 * -# Delete node/edge - use delete option from context menu
 * -# Enter text in node - double click on node to enable its text editor
 * -# Move node - press left mouse button on it and drag
 * -# Create edge control point - double click on edge
 * -# Zoom view - press '-' and '+' or use mouse wheel
 * -# Invoke auto layout - select 'Run Layout' from View menu or simply press F5 
 */
#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "gui_impl.h"

/**
 * Graph for graphics. Graph model layer of GraphView.
 * @ingroup GUIGraph
 */
class GGraph: public AuxGraph
{
    GraphView *view_p;
	Marker nodeTextIsShown;
	QList< GNode* > sel_nodes;
	QList< GEdge* > sel_edges;
public:
    /** Constructor */
    inline GGraph( GraphView *v): view_p( v)
	{
		nodeTextIsShown = newMarker();
	}
    
    /** Destructor */
    ~GGraph();

    /** New graphical node */
    GNode* newNode();
    /** New graphical node */
    GNode* newNode( QDomElement e);
    /** New graphical edge */
    GEdge* newEdge( GNode* pred, GNode* succ);
    /** New graphical edge */
    GEdge* newEdge( GNode* pred, GNode* succ, QDomElement e);
    
    /** Reimplementation of newEdge virtual function of base class */
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ)
    {
        return ( AuxEdge*)newEdge( static_cast< GNode *>( pred),
                                    static_cast< GNode *> (succ));
    }
    /** Reimplementation of newEdge virtual function of base class */
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ, QDomElement e)
    {
        return ( AuxEdge*)newEdge( static_cast< GNode *>( pred),
                                    static_cast< GNode *> (succ), e);
    }
    /** Node creation reimplementaiton */
    virtual void * CreateNode( AuxGraph *graph_p, int _id)
    {
        GNode* node_p = new GNode( static_cast<GGraph *>(graph_p), _id);
        return node_p;
    }
    /** Edge creation reimplementation */
    virtual void * CreateEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ)
    {
        return new GEdge(  static_cast<GGraph *>( graph_p), _id,
                              static_cast<GNode *>( _pred), 
                              static_cast<GNode *>( _succ));
    }
    /** Get graph's first edge */
    inline GEdge* firstEdge() 
    {
        return static_cast< GEdge *>( AuxGraph::firstEdge());
    }
    /** Get graph's first node */
    inline GNode* firstNode()
    {
        return static_cast< GNode *>( AuxGraph::firstNode());
    }
    /**
     * Run layout procedure
     */
    void doLayout();

    /** Get corresponding graph view widget */
    inline GraphView *view() const
    {
        return view_p;
    }
	/**
     * Add node to selection
     */
    inline void selectNode( GNode *n)
    {
        sel_nodes << n;
    }
	/**
	 * Clear list of selected nodes
	 */
	inline void emptySelection()
	{
		sel_nodes.clear();
		sel_edges.clear();
	}
	/**
     * Add node to selection
     */
    inline void selectEdge( GEdge *e)
    {
        sel_edges << e;
    }
	/**
	 * Delete scheduled nodes
	 */
	void deleteNodes()
	{
		foreach (GNode *n, sel_nodes)
		{
			delete n;
		}
	}

	/** Delete edge with all of the edge controls on it */
	void deleteEdgeWithControls( GEdge *e);

	/**
	 * Delete scheduled edges
	 */
	void deleteEdges()
	{
		foreach (GEdge *e, sel_edges)
		{
			deleteEdgeWithControls( e);
		}
	}
    /**
     * Create self edge on selected node
     */
    void createSelfEdge()
    {
        if ( !sel_nodes.isEmpty())
        {
            GNode *n = sel_nodes.first();
            newEdge( n, n);
        }
    }
	
};

/**
 * Graph visualization class
 */
class GraphView: public QGraphicsView
{
    Q_OBJECT; /** For MOC */
private:
    QPoint src;
    QPoint dst;
    bool createEdge;
	bool show_menus;
    GNode *tmpSrc;
    GGraph * graph_p;
    qreal zoom_scale;

    QList< NodeItem* > del_node_items;
    QList< EdgeItem* > del_edge_items;

	/** Actions */
	QAction *deleteItemAct;
    QAction *createSelfEdgeAct;
	
	/** Context menus */
	QMenu *nodeItemMenu;
	QMenu *edgeItemMenu;
private:
	void createActions();
	void createMenus();
signals:
    /** Signal that node is clicked */
    void nodeClicked( GNode *n);
public slots:
	/** Delete one item	 */
	void deleteSelected();
	/** create self edge on selected node */
	void createSESelected();

public:
    /** Constants */
#ifdef _DEBUG
    static const int MAX_DELETED_ITEMS_COUNT = 100;
#else
    static const int MAX_DELETED_ITEMS_COUNT = 10000;
#endif
    /** Constructor */
    GraphView();
    /** Destructor */
    ~GraphView();
    /** Show text of the clicked node */
    inline void showNodeText( GNode * n)
    {
        emit nodeClicked( n);
    }
    /** Get pointer to model graph */
    inline GGraph *graph() const
    {
        return graph_p;
    }
	/** Get context menu for nodes */
	inline QMenu *nodeMenu() const
	{
		return nodeItemMenu;
	}
	/** Get context menu for edges */
	inline QMenu *edgeMenu() const
	{
		return edgeItemMenu;
	}

    /** Create menu for particular node */
    QMenu* createMenuForNode( GNode *n);

	void dragEnterEvent(QDragEnterEvent *event);

	void dropEvent(QDropEvent *event);
	 
	void dragMoveEvent(QDragMoveEvent *event);

    /** draw background reimplementation */
    void drawBackground(QPainter *painter, const QRectF &rect);
    /** Mouse double click event handler reimplementation */
    void mouseDoubleClickEvent(QMouseEvent *event);
    /** Mouse press event handler reimplementation */
    void mousePressEvent(QMouseEvent *event);
    /** Mouse move event handler reimplementation */
    void mouseMoveEvent(QMouseEvent *event);
    /** Mouse release event handler reimplementation */
    void mouseReleaseEvent(QMouseEvent *event);
   
	/** Mouse wheel event handler reimplementation */
    void wheelEvent(QWheelEvent *event);
    /** Zoom the view in */
    void zoomIn();
    /** Zoom the view out */
    void zoomOut();
    /** Restore original zoom */
    void zoomOrig();
    /** Do the transofrmation( scale) */
	void updateMatrix();
    /** Check if we are in the process of the edge creation */
    inline bool isCreateEdge() const
    {
        return createEdge;
    }
	/** Check if we are in the process of the edge creation */
    inline bool isShowContextMenus() const
    {
        return show_menus;
    }
    /** Set the state of ege creation */
    inline void SetCreateEdge( bool val)
    {
        createEdge = val;
    }
    /** Save the pointer to source node for new edge */
    inline void SetTmpSrc( GNode* node)
    {
        tmpSrc = node;
    }
    /** Get the pointer to source node of new edge */
    inline GNode* GetTmpSrc()
    {
        return tmpSrc;
    }

    /**
     * Schedule node item for deletion
     */
    void deleteLaterNodeItem( NodeItem *item)
    {
        del_node_items << item;
        checkDelItems();
    }
    /**
     * Schedule edge item for deletion
     */
    void deleteLaterEdgeItem( EdgeItem *item)
    {
        del_edge_items << item;
        checkDelItems();
    }
    
    /**
     * Delete items that have been disconnected from scene
     */
    void deleteItems();
    
	/**
	 * Find node by its ID from dump
	 */
	bool findNodeById( int id);

    /** 
     * Check that we haven't exceeded the max amount of deleted items
     */
    inline void checkDelItems()
    {
        int item_count = del_node_items.count() + del_edge_items.count();
        if ( item_count >= MAX_DELETED_ITEMS_COUNT)
        {
            /** deleteItems(); !!! FIXME: MEMORY LEACKAGE( yes, not potential... known leakage) */
        }
    }
    
};

#endif