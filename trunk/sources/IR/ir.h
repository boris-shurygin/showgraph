/**
 * @file: ir.h 
 * Description of expressions' graph
 * @author Boris Shurygin
 */
/*
 * IR interface
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
protected:
    /** Node creation reimplementaiton */
    virtual Node * createNode( int _id)
    {
        Expr* node_p = new ( node_pool) Expr( this, _id);
        return node_p;
    }
    /** Edge creation reimplementation */
    virtual Edge * createEdge( int _id, GNode *_pred, GNode* _succ)
    {
        return new ( edge_pool) Dep(  this, _id,
                                      static_cast<Expr *>( _pred), 
                                      static_cast<Expr *>( _succ));
    }
public:
    /** Constructor */
    IR( GraphView *v, bool create_pools);
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

/**
 * @brief Compiler intermediate representation view
 * Used as a view class for data structures like DFG or any other graph
 * that is built with individual expressions/operations represented by nodes
 * and dependencies between the represented by edges.
 */
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