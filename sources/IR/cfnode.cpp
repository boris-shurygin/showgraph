/**
 * @file: cf_node.cpp 
 * Control flow graph node implementation
 */
/* 
 * IR for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "ir_impl.h"

/** We can't create nodes separately, do it through newNode method of graph */
CFNode::CFNode( CFG *graph_p, int _id):
    GNode( ( GGraph *)graph_p, _id)
{

}

/** Contructor of node with specified position */
CFNode::CFNode( CFG *graph_p, int _id, QPointF _pos):
    GNode( ( GGraph *)graph_p, _id)
{

}

/**
 * Destructor for node - removes edge controls on incidient edges and disconnects item from scene
 */
CFNode::~CFNode()
{

}
