/**
 * File: graph_w.h - Graph Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_W_H
#define GRAPH_W_H

#include "gui_impl.h"

class GraphView: public AuxGraph, public QGraphicsView
{
    QPoint src;
    QPoint dst;
    bool createEdge;
    GNode *tmpSrc;
public:
    /** Constructor */
    GraphView();
    ~GraphView();

    void drawBackground(QPainter *painter, const QRectF &rect);
    
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

    /** Graph part */
    virtual void * CreateNode( AuxGraph *graph_p, int _id)
    {
        GNode* node_p = new GNode( static_cast<GraphView *>(graph_p), _id);
        return node_p;
    }
    virtual void * CreateEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ)
    {
        return new GEdge(  static_cast<GraphView *>( graph_p), _id,
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
    void doLayout()
    {
        AuxGraph::doLayout();
        for ( GNode* n = firstNode();
              isNotNullP( n);
              n = n->nextNode())
        {
            n->item()->setPos( n->modelX(), n->modelY());
        }
    }
};

#endif