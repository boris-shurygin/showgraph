/**
 * @file: cfg.cpp
 * Implementation of CFG
 * @author Boris Shurygin
 */
/**
 * IR implementation
 * Copyright (C) 2009  Boris Shurygin
 */
#include "ir_impl.h"

/** Constructor */
CFG::CFG( GraphView *v, bool create_pools): GGraph( v, false)
{
    /** Pools' creation routine */
    if ( create_pools)
    {
        node_pool = new FixedPool< CFNode>();
        edge_pool = new FixedPool< CFEdge>();
    }
}

/** Destructor */
CFG::~CFG()
{

}

CFNode*
CFG::newNode()
{
    CFNode* n = static_cast< CFNode *>( GGraph::newNode());
    return n;
}

CFEdge*
CFG::newEdge( CFNode* pred, CFNode* succ)
{
    CFEdge* e = 
        static_cast< CFEdge *>( GGraph::newEdge( (GNode *)pred, (GNode *)succ));
    return e;
}

CFNode*
CFG::newNode( QDomElement e)
{
    CFNode* n =  static_cast< CFNode *>( GGraph::newNode( e));
    view()->scene()->addItem( n->item());
    return n;
}

CFEdge*
CFG::newEdge( CFNode* pred, CFNode* succ, QDomElement e)
{
    CFEdge* edge_p = 
        static_cast< CFEdge *>( GGraph::newEdge( (GNode *)pred, (GNode *)succ, e));
    return edge_p;
}