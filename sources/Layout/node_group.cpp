/**
 * File: node_group.cpp - Implementation of node group class
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "layout_iface.h"

/**
 * Compare orders of nodes
 */
bool compareBc( AuxNode* node1,
                AuxNode* node2)
{
    return ( node1->bc() < node2->bc());
}

/**
 * Constructor of group from a node.
 * Coordinates are computed with respect to pass direction
 */
NodeGroup::NodeGroup( AuxNode *n,   // Parent node
                      GraphDir dir, // Pass direction
                      bool first_pass) // If this is the first run
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
        if ( !e->isInverted())
        {
            num_peers++;
            sum+= ( e->node( rdir)->modelX() + ( e->node( rdir)->width() / 2));
        }
    }
    for ( AuxEdge* e = n->firstEdgeInDir( dir);
          isNotNullP( e);
          e = e->nextEdgeInDir( dir))
    {
        if ( e->isInverted())
        {
            num_peers++;
            sum+= ( e->node( dir)->modelX() + ( e->node( dir)->width() / 2));
        }
    }
    /** Barycenter heuristic */
    double center = 0;
    if ( num_peers > 0)
    {
        center = sum / num_peers;
    } else if ( !first_pass)
    {
        center = n->modelX() + n->width() / 2;
    }
    n->setBc( center);
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
    int num = 0;
    qreal barycenter = 0;
    
    qSort( node_list.begin(), node_list.end(), compareBc);
    
    foreach ( AuxNode* node, node_list)
    {
        width += node->spacing( prev_type);
        barycenter += width + node->width() / 2; 
        width += node->width();
        prev_type = node->type();
        num++;
    }
    barycenter = barycenter / num;

    /* 3. set borders */
    setLeft( center - barycenter);
    setRight( left() + width);
    //out("Width %e, center %e, barycenter %e", width, center, barycenter);

}

/**
 * Place nodes withing the group
 */
void NodeGroup::placeNodes()
{
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;
    
    qreal curr_left = left();
    foreach ( AuxNode* node, node_list)
    {
        curr_left += node->spacing( prev_type);
        node->setX( curr_left);
        curr_left += node->width();
        prev_type = node->type();
    }
}

/**
 * Place nodes withing the group
 */
void NodeGroup::placeNodesFinal( GraphDir dir)
{
    AuxNodeType prev_type = AUX_NODE_TYPES_NUM;
    
    qreal curr_left = left();
    //out("Node placement: from %e to %e", left(), right());

    foreach ( AuxNode* node, node_list)
    {
        //out("Node %d", node->id()); 
        curr_left += node->spacing( prev_type);
        node->setX( curr_left);
        curr_left += node->width();
        prev_type = node->type();
        node->setY( node->modelY() - node->height() / 2 );
    }
}