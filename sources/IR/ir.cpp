/**
 * @file: ir.cpp 
 * Implementation of IR
 * @author Boris Shurygin
 */
/**
 * IR implementation
 * Copyright (C) 2009  Boris Shurygin
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