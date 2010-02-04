/**
 * @file: cfedge.cpp
 * Implementation of CFEdge
 * @author Boris Shurygin
 */
/**
 * IR implementation
 * Copyright (C) 2009  Boris Shurygin
 */
#include "ir_impl.h"

CFEdge::CFEdge( CFG *graph_p, int _id, CFNode *_pred, CFNode* _succ):
    GEdge( (GGraph *)graph_p, _id, (GNode *)_pred, (GNode *)_succ)
{

}
   
CFEdge::~CFEdge()
{

}

CFNode * 
CFEdge::node( GraphDir dir) const 
{
    return static_cast< CFNode *>(AuxEdge::node( dir));
}

CFNode * 
CFEdge::realNode( GraphDir dir) const 
{
    return static_cast< CFNode *>(AuxEdge::realNode( dir));
}

CFG *
CFEdge::graph() const
{
    return static_cast< CFG *>( AuxEdge::graph());
}