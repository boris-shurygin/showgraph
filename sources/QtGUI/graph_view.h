/**
 * File: graph_w.h - Graph View class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "gui_impl.h"

/**
 * Graph for graphics. Graph model layer of GraphView.
 */
class GGraph: public AuxGraph
{
    GraphView *view_p;
public:
    /** Constructor */
    inline GGraph( GraphView *v): view_p( v){};
    
    /** Destructor */
    ~GGraph();

    /** New node/edge overloads */
    GNode* newNode();
    GNode* newNode( QDomElement e);
    GEdge* newEdge( GNode* pred, GNode* succ);
    GEdge* newEdge( GNode* pred, GNode* succ, QDomElement e);
    
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ)
    {
        return ( AuxEdge*)newEdge( static_cast< GNode *>( pred),
                                    static_cast< GNode *> (succ));
    }
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ, QDomElement e)
    {
        return ( AuxEdge*)newEdge( static_cast< GNode *>( pred),
                                    static_cast< GNode *> (succ), e);
    }
    /** Graph part */
    virtual void * CreateNode( AuxGraph *graph_p, int _id)
    {
        GNode* node_p = new GNode( static_cast<GGraph *>(graph_p), _id);
        return node_p;
    }
    virtual void * CreateEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ)
    {
        return new GEdge(  static_cast<GGraph *>( graph_p), _id,
                              static_cast<GNode *>( _pred), 
                              static_cast<GNode *>( _succ));
    }
    inline GEdge* firstEdge() 
    {
        return static_cast< GEdge *>( AuxGraph::firstEdge());
    }
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
    /** View connection */
    inline GraphView *view() const
    {
        return view_p;
    }
};

/**
 * Graph visualization class
 */
class GraphView: public QGraphicsView
{
    Q_OBJECT; /** For MOC */

    QPoint src;
    QPoint dst;
    bool createEdge;
    GNode *tmpSrc;
    GGraph * graph_p;

    QList< NodeItem* > del_node_items;
    QList< EdgeItem* > del_edge_items;
    
signals:
    void nodeClicked( GNode *n);

public:
    /** Constants */
#ifdef _DEBUG
    static const int MAX_DELETED_ITEMS_COUNT = 100;
#else
    static const int MAX_DELETED_ITEMS_COUNT = 10000;
#endif
    /** Constructor */
    GraphView();
    ~GraphView();

    inline void showNodeText( GNode * n)
    {
        emit nodeClicked( n);
    }
    inline GGraph *graph() const
    {
        return graph_p;
    }
    
    void drawBackground(QPainter *painter, const QRectF &rect);
    
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    
    inline bool IsCreateEdge() const
    {
        return createEdge;
    }
    inline void SetCreateEdge( bool val)
    {
        createEdge = val;
    }
    inline void SetTmpSrc( GNode* node)
    {
        tmpSrc = node;
    }
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