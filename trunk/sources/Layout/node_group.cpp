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
