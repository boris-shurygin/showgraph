/**
 * @file: ir.cpp 
 * Implementation of IR
 * @author Boris Shurygin
 */
/**
 * IR implementation
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
#include "ir_impl.h"

/** Constructor */
IR::IR( GraphView *v, bool create_pools): GGraph( v, false)
{
    /** Pools' creation routine */
    if ( create_pools)
    {
        node_pool = new FixedPool< Expr>();
        edge_pool = new FixedPool< Dep>();
    }
}

/** Destructor */
IR::~IR()
{

}

Expr*
IR::newNode()
{
    Expr* n = static_cast< Expr *>( GGraph::newNode());
    return n;
}

Dep*
IR::newEdge( Expr* pred, Expr* succ)
{
    Dep* e = 
        static_cast< Dep *>( GGraph::newEdge( (GNode *)pred, (GNode *)succ));
    return e;
}

Expr*
IR::newNode( QDomElement e)
{
    Expr* n =  static_cast< Expr *>( GGraph::newNode( e));
    view()->scene()->addItem( n->item());
    return n;
}

Dep*
IR::newEdge( Expr* pred, Expr* succ, QDomElement e)
{
    Dep* edge_p = 
        static_cast< Dep *>( GGraph::newEdge( (GNode *)pred, (GNode *)succ, e));
    return edge_p;
}

IRView::IRView(): GraphView()
{
    createActions();
}

void IRView::createActions()
{
    showPrevAct = new QAction(tr("Prev"), this);
    connect( showPrevAct, SIGNAL(triggered()), this, SLOT( showExprPrev()));

    showNextAct = new QAction(tr("Next"), this);
    connect( showNextAct, SIGNAL(triggered()), this, SLOT( showExprNext()));
}

/** Create menu for particular node */
QMenu* IRView::createMenuForNode( GNode *n)
{
    QMenu *menu = GraphView::createMenuForNode( n);
    menu->addSeparator();
    menu->addAction( showPrevAct);
    menu->addAction( showNextAct);
    return menu;
}

void IR::showExprPrev()
{
    if ( !sel_nodes.isEmpty())
    {
        Expr *expr = static_cast< Expr *>( sel_nodes.first());
        Expr *prev = expr->prev();
        if ( isNotNullP( prev))
        {
            emptySelection();
            selectNode( prev);
            if ( view()->isContext())
            {
                view()->findContext();
            }
            view()->focusOnNode( prev, true);
        }
    }
}

void IR::showExprNext()
{
    if ( !sel_nodes.isEmpty())
    {
        Expr *expr = static_cast< Expr *>( sel_nodes.first());
        Expr *next = expr->next();
        if ( isNotNullP( next))
        {
            emptySelection();
            selectNode( next);
            if ( view()->isContext())
            {
                view()->findContext();
            }
            view()->focusOnNode( next, true);
        }
    }
}

void IRView::showExprPrev()
{
    graph()->showExprPrev();
}
void IRView::showExprNext()
{
    graph()->showExprNext();
}