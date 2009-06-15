/**
 * File: node_group.h - Abstraction that aids multiple nodes manipulation inside a level
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_GROUP_H
#define NODE_GROUP_H

/**
 * Group of nodes
 */
class NodeGroup
{
    QList< AuxNode *> node_list;
    double border_left;
    double border_right;
    float group_weight;
public:
    inline void init()
    {
        border_left = 0;
        border_right = 0;
        group_weight = 0;        
    }
    /** Get/Set routines */
    inline float weight() const
    {
        return group_weight;
    }
    inline qreal left() const
    {
        return border_left;
    }
    inline qreal right() const
    {
        return border_right;
    }
    QList<AuxNode *> nodes() const
    {
        return node_list;
    }

    /** Work with node list */
    inline void addNode( AuxNode *node)
    {
        node_list.push_back( node);
        group_weight += 1;
    }

    /**
     * Constructors
     */
    /** Default */
    NodeGroup() : node_list()
    {
        init();
    }
    /** Constructor of group from a node */
    NodeGroup( AuxNode *n, GraphDir dir);

    /** Multiple group actions */
    inline bool interleaves( NodeGroup *grp) const
    {
        return !( left() > grp->right() 
                  || right() < grp->left());
    }

    void merge( NodeGroup *grp)
    {
        // Add nodes from group on the left
        node_list += grp->nodes();

        // Recalculate border coordinates
    }
};
#endif