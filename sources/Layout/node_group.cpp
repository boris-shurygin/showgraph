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
        sum+=e->node( rdir)->x();
    }
    /** Barycenter heuristic */
    double center = sum / num_peers;
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
        switch ( prev_type)
        {
            case AUX_NODE_SIMPLE:
                if ( node->type() == AUX_NODE_SIMPLE)
                {
                    width += NODE_NODE_MARGIN;
                } else
                {
                    width += NODE_CONTROL_MARGIN;
                }
                break;
            case AUX_EDGE_CONTROL:
                if ( node->type() == AUX_NODE_SIMPLE)
                {
                    width += NODE_CONTROL_MARGIN;
                } else
                {
                    width += CONTROL_CONTROL_MARGIN;
                }
                break;
            case AUX_NODE_TYPES_NUM:
                break;
        }
        width += node->width();

        /* 3. set borders */
        setLeft( center - width / 2);
        setRight( center + width / 2 );
    }
}