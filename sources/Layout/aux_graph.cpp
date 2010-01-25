/**
 * @file: aux_graph.cpp
 * Implementation of auxiliary graph
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "layout_iface.h"

/**
 * Default constructor
 */
AuxGraph::AuxGraph(): ranking_valid( false), levels() 
{
    ranking = newNum();
    order = newNum();
}

/**
 * Initialize levels
 */
void
AuxGraph::initLevels( Rank max_level)
{
    levels.resize( max_level + 1);
    for ( Rank i = 0; i <= max_level; i++)
    {
        levels[ i] = new Level( i);
    } 
}

/**
 * Delete levels
 */
void
AuxGraph::deleteLevels()
{
    for ( int i = 0; i < levels.size(); i++)
    {
        delete levels[ i];
    } 
}


/**
 * Destructor. Cleans up level info
 */
AuxGraph::~AuxGraph()
{
    deleteLevels();
    freeNum( ranking);
    freeNum( order);
}

/**
 * Get root node of the graph
 */
AuxNode*
AuxGraph::rootNode()
{
    Level* root_level = levels[ 0];

    if( isNullP( root_level) || isNullP( firstNode()))
    {
        return firstNode();
    }
    /** Use median heuristic to select root node */
    qreal center;
    foreach ( AuxNode* node, root_level->nodes())
    {
        center = node->modelX() + node->width()/ 2;
    }
    center = center / root_level->nodes().count();
    
    AuxNode* prev = NULL;
    AuxNode* node = NULL;
    
    foreach ( node, root_level->nodes())
    {
        if ( center <= node->modelX() + node->width()/ 2)
        {
            qreal delta_curr = node->modelX() + node->width()/ 2 - center;
            if ( isNotNullP( prev)
                 && ( delta_curr > center - prev->modelX() - prev->width()/ 2))
            {
                return prev;
            } else
            {
                return node;
            }
        }
        prev = node;
    }
    return firstNode();
}