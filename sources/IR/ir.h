/**
 * @file: ir.h 
 * Description of expressions' graph
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef IR_H
#define IR_H

#include "ir_iface.h"
/**
 * Intermediate representation
 */
class IR: public GGraph
{
    CFG *cf_graph;
public:
    /** Constructor */
    IR( GraphView *v);
    /** Destructor */
    ~IR();
    /** New graphical node */
    Expr* newNode();
    /** New graphical node */
    Expr* newNode( QDomElement e);
    /** New graphical edge */
    Dep* newEdge( Expr* pred, Expr* succ);
    /** New graphical edge */
    Dep* newEdge( Expr* pred, Expr* succ, QDomElement e);
    
    /** Reimplementation of newEdge virtual function of base class */
    GEdge* newEdge( GNode * pred, GNode *succ)
    {
        return ( GEdge*)newEdge( static_cast< Expr *>( pred),
                                    static_cast< Expr *> (succ));
    }
    /** Reimplementation of newEdge virtual function of base class */
    GEdge* newEdge( GNode * pred, GNode *succ, QDomElement e)
    {
        return ( GEdge*)newEdge( static_cast< Expr *>( pred),
                                    static_cast< Expr *> (succ), e);
    }
    /** Node creation reimplementaiton */
    virtual void * CreateNode( AuxGraph *graph_p, int _id)
    {
        Expr* node_p = new Expr( static_cast<IR *>(graph_p), _id);
        return node_p;
    }
    /** Edge creation reimplementation */
    virtual void * CreateEdge( AuxGraph *graph_p, int _id, GNode *_pred, GNode* _succ)
    {
        return new Dep(  static_cast<IR *>( graph_p), _id,
                              static_cast<Expr *>( _pred), 
                              static_cast<Expr *>( _succ));
    }
    /** Get graph's first edge */
    inline Dep* firstEdge() 
    {
        return static_cast< Dep *>( AuxGraph::firstEdge());
    }
    /** Get graph's first node */
    inline Expr* firstNode()
    {
        return static_cast< Expr *>( AuxGraph::firstNode());
    }
    /** Get ir's first dep edge */
    inline Dep* firstDep() 
    {
        return static_cast< Dep *>( AuxGraph::firstEdge());
    }
    /** Get ir's first expression */
    inline Expr* firstExpr()
    {
        return static_cast< Expr *>( AuxGraph::firstNode());
    }
    /** Get CFG associated with IR */
    inline CFG *cfg() const
    {
        return cf_graph;
    }
    /** Set CF graph for IR */
    inline void setCFG( CFG *g)
    {
        cf_graph = g;
    }
    void showExprPrev();
    void showExprNext();
};

class IRView: public GraphView
{
    Q_OBJECT;
    
    QAction *showPrevAct;
    QAction *showNextAct;
public:
    IRView();
    /** Get IR */
    inline IR *graph() const
    {
        return static_cast< IR* >( GraphView::graph());
    }
public slots:
    void showExprPrev();
    void showExprNext();
protected:
	/** Create actions for node and edge dropdown menus */
    void createActions();
    /** Create menu for particular node */
    virtual QMenu* createMenuForNode( GNode *n);

};
#endif /* IR_H */