/**
 * @file: dep.cpp
 * Implementation of dependence
 * @author Boris Shurygin
 */
/**
 * IR implementation
 * Copyright (C) 2009  Boris Shurygin
 */
#include "ir_impl.h"

Dep::Dep( IR *graph_p, int _id, Expr *_pred, Expr* _succ):
    GEdge( (GGraph *)graph_p, _id, (GNode *)_pred, (GNode *)_succ)
{

}
   
Dep::~Dep()
{

}

Expr * 
Dep::node( GraphDir dir) const 
{
    return static_cast< Expr *>(AuxEdge::node( dir));
}

Expr * 
Dep::realNode( GraphDir dir) const 
{
    return static_cast< Expr *>(AuxEdge::realNode( dir));
}

IR *
Dep::graph() const
{
    return static_cast< IR *>( AuxEdge::graph());
}