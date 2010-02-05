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
IR::IR( GraphView *v): GGraph( v)
{

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

