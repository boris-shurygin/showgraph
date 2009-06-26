/**
 * File: layout.cpp - Layout algorithms implementation file
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "layout_iface.h"

/**
 * Compare orders of nodes
 */
bool compareOrders( AuxNode* node1,
                    AuxNode* node2)
{
    return ( node1->order() < node2->order());
}

/**
 * compare center coordinates of group
 */
bool compareGroups( NodeGroup* g1,
                    NodeGroup* g2)
{
    /*
     * center = g.left + width / 2 = g.left + ( g.right - g.left) / 2 = ( g.left + g.right)/2
     *
     *                g1.center < g2.center 
     * (g1.left + g1.right) / 2 < (g2.left + g2.right) / 2
     *       g1.left + g1.right < g2.left + g2.right
     */
    return ( g1->left() + g1->right() < g2->left() + g2->right());
}

/**
 * Sort nodes in a level with respect to their order
 */
void Level::sortNodesByOrder()
{
    qSort( node_list.begin(), node_list.end(), compareOrders);
}

/**
 * Arranges nodes using group merge algorithm.
 * NodeGroup is a group of nodes which interleave if we apply baricentric heuristic directly.
 * These nodes are placed within group borders. If two groups interleave they are merged.
 * Arrangement is performed iteratively starting with groups that have one node each.
 */
void Level::arrangeNodes( GraphDir dir, bool commit_placement, bool first_pass)
{
    QList< NodeGroup *> list;
    foreach ( AuxNode* node, node_list)
    {
        NodeGroup* group = new NodeGroup( node, dir, first_pass);
        list.push_back( group);
    }
    /** Sort groups with respect to their coordinates */
    qSort( list.begin(), list.end(), compareGroups);
    
    QLinkedList< NodeGroup *> groups;
    
    foreach( NodeGroup *group, list)
    {
        groups.push_back( group);
    }
    QLinkedList< NodeGroup *>::iterator it = groups.begin();

    /**
     * For each group
     */
    while( groups.count())
    {
        /*
         * 1. Look at the group to the right and left and see they interleave
         *    if they do -> merge groups and repeat
         */
        NodeGroup* grp = *it;
        QLinkedList< NodeGroup *>::iterator it_right = it;
        it_right++;
        bool no_merge = true; 

        /** Group to the left */
        if ( it != groups.begin())
        {
            QLinkedList< NodeGroup *>::iterator it_left = it;
            it_left--;
            NodeGroup* left_grp = *it_left;
            if ( grp->interleaves( left_grp))
            {
                groups.erase( it_left);
                grp->merge( left_grp);
                no_merge = false;
                delete ( left_grp);
            }
        }
        /** Group to the right */
        if ( it_right != groups.end())
        {
            NodeGroup* right_grp = *it_right;
            if ( grp->interleaves( right_grp))
            {
                groups.erase( it_right);
                grp->merge( right_grp);
                no_merge = false;
                delete ( right_grp);
            }    
        }
        /** Proceed to the next group */
        if ( no_merge)
            it++;
        /** End loop if we have processed all groups and merged everything we could */
        if ( it == groups.end())
            break;
    }

    if ( commit_placement)
    {
        /** Assign coordinates to nodes */
        for ( it = groups.begin(); it != groups.end(); it++)
        {
            NodeGroup *grp = *it;
            grp->placeNodesFinal( dir);
            delete grp;
        }
    } else 
    {
        /** Assign coordinates to nodes */
        for ( it = groups.begin(); it != groups.end(); it++)
        {
            NodeGroup *grp = *it;
            grp->placeNodes();
            delete grp;
        }
    }
}

/**
 * Assign order to nodes by numbering in a DFS traversal
 */
void AuxGraph::orderNodesByDFS()
{
    /** Structure used for dfs traversal */
    struct DfsStepInfo
    {
        AuxNode *node; // Node in consideration
        AuxEdge *edge; // Next edge

        /* Constructor */
        DfsStepInfo( AuxNode *n)
        {
            node = n;
            edge = n->firstSucc();
        }
    };

    Marker m = newMarker(); // Marker for visiting nodes
    QStack< DfsStepInfo *> stack;
    GraphNum num = 0;
    
    /* Fill stack with nodes that have no predecessors */
    for ( AuxNode *n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        if ( isNullP( n->firstPred()) && !n->isMarked( m))
        {
            n->setOrder( num++);
            stack.push( new DfsStepInfo( n));

            /* Walk graph with marker and perform classification */
            while( !stack.isEmpty())
            {
                DfsStepInfo *info = stack.top();
                AuxNode *node = info->node;
                AuxEdge *edge = info->edge;
                
                if ( isNotNullP( edge)) // Add successor to stack
                {
                    AuxNode* succ_node = edge->succ();
                    info->edge = edge->nextSucc();
                    
                    if ( succ_node->mark( m))
                    {
                        stack.push( new DfsStepInfo( succ_node));
                        succ_node->setOrder( num++);
                    }
                } else // We're done with this node
                {
                    delete info;
                    stack.pop();
                }
            }
        }
    }
    freeMarker( m);
}

/**
 * Try to reduce number of edge crossings between levels
 */
void AuxGraph::reduceCrossings()
{
    /** Perform numeration and sort nodes to avoid tree edges crossings */
    orderNodesByDFS();

    for ( int i = 0; i < levels.size(); i++)
    {
        Level* level = levels[ i];
        level->sortNodesByOrder();
    }
}

/**
 * Assign X coordinates to the nodes
 */
void AuxGraph::arrangeHorizontally()
{
    /* Descending pass */
    for ( int i = 0; i < levels.size(); i++)
    {
        levels[ i]->arrangeNodes( GRAPH_DIR_DOWN, false, true);
    }
    
    /* Ascending pass */
    for ( int i = levels.size() - 1; i >= 0; i--)
    {
        levels[ i]->arrangeNodes( GRAPH_DIR_UP, true, false);
    }
}