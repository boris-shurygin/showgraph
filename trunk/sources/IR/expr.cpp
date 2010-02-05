/**
 * @file: expr.cpp 
 * IR node implementation
 */
/* 
 * IR for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "ir_impl.h"

/** We can't create nodes separately, do it through newNode method of graph */
Expr::Expr( IR *graph_p, int _id):
    GNode( ( GGraph *)graph_p, _id)
{

}

/** Contructor of node with specified position */
Expr::Expr( IR *graph_p, int _id, QPointF _pos):
    GNode( ( GGraph *)graph_p, _id)
{

}

/**
 * Destructor for node - removes edge controls on incidient edges and disconnects item from scene
 */
Expr::~Expr()
{

}
