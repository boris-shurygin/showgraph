/**
 * @file: graph_view.h 
 * Graph View class definition.
 * @defgroup GUIGraph Graph Visualization System
 * @ingroup GUI
 */
/* GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
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
	QList< GNode* > sel_nodes;
	QList< GEdge* > sel_edges;
public:
    /** Constructor */
    inline GGraph( GraphView *v): view_p( v){};
    
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
    inline void doLayout()
    {
        AuxGraph::doLayout();
        for ( GNode* n = firstNode();
              isNotNullP( n);
              n = n->nextNode())
        {
            n->item()->setPos( n->modelX(), n->modelY());
        }
    }
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
	/**
	 * Delete scheduled edges
	 */
	void deleteEdges()
	{
		foreach (GEdge *e, sel_edges)
		{
			delete e;
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
    GNode *tmpSrc;
    GGraph * graph_p;
    qreal zoom_scale;

    QList< NodeItem* > del_node_items;
    QList< EdgeItem* > del_edge_items;

	/** Actions */
	QAction *deleteItemAct;
	
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
	/**
	 * Delete one item
	 */
	void deleteSelected();

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
    inline bool IsCreateEdge() const
    {
        return createEdge;
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
     * Check that we haven't exceeded the max amount of deleted items
     */
    inline void checkDelItems()
    {
        int item_count = del_node_items.count() + del_edge_items.count();
        if ( item_count >= MAX_DELETED_ITEMS_COUNT)
        {
            deleteItems();
        }
    }
    
};

#endif