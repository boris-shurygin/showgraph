/**
 * File: node_group.cpp - Implementation of node group class
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "layout_iface.h"

/**
 * Constructor of group from a node.
 * Coordinates are computed with respect to pass direction
 */
NodeGroup::NodeGroup( AuxNode *n,   // Parent node
                      GraphDir dir) // Pass direction
                      : node_list()
{
    init();
    addNode( n);
    /** Compute coordinates */
    double sum = 0;
    int num_peers = 0;
    /**
     * On descending pass we compute center coordinate with respect to coordinates of predecessors,
     * on ascending - we look at successors
     */
    GraphDir rdir = RevDir( dir);
    for ( AuxEdge* e = n->firstEdgeInDir( rdir);
          isNotNullP( e);
          e = e->nextEdgeInDir( rdir))
    {
        num_peers++;
        sum+= ( e->node( rdir)->x() + ( e->node( rdir)->width() / 2));
    }
    /** Barycenter heuristic */
    double center = 0;
    if ( num_peers > 0)
    {
        center = sum / num_peers;
    }
    border_left = center - n->width() / 2;
    border_right = center + n->width() / 2;
}

/**
 * Merge two groups correcting borders and nodes list of resulting group
 */
void NodeGroup::merge( NodeGroup *grp)
{
    /** Add nodes from group on the left */
    node_list += grp->nodes();
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;

    /** Recalculate border coordinates */
    /* 1. calculate center coordinate */
    qreal center = ( right() + left()) / 2; 
    qreal merged_center = ( grp->right() + grp->left()) / 2; 
    center = ( center + merged_center) / 2;
    
    /* 2. calculate width */
    qreal width = 0;
    foreach ( AuxNode* node, node_list)
    {
        width += node->spacing( prev_type);
        width += node->width();
        prev_type = node->type();
    }

    /* 3. set borders */
    setLeft( center - width / 2);
    setRight( center + width / 2 );
}

/**
 * Place nodes withing the group
 */
void NodeGroup::placeNodes()
{
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;
    
    qreal curr_left = left();
    //out("Node placement: from %e to %e", left(), right());

    foreach ( AuxNode* node, node_list)
    {
        //out("Node %d", node->id()); 
        curr_left += node->spacing( prev_type);
        node->setX( curr_left);
        if( node->isSimple())
        {
            node->node()->setPos( curr_left, node->y() - node->height() / 2 );
        } else if ( node->isEdgeControl())
        {
            EdgeItem* edge = node->edge();
            EdgeSegment* seg;
            
            if ( edge->isInverted())
            {
                seg = edge->srcCtrl()->succ();
            } else
            {
                seg = edge->dstCtrl()->pred();
            }
            EdgeControl* ctrl = seg->addControl( QPointF( curr_left, node->y() - node->height() / 2));
            ctrl->setFixed();
            edge->adjust();
        }
        curr_left += node->width();
        prev_type = node->type();
    }
}