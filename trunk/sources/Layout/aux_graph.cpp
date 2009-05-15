/**
 * File: aux_graph.cpp - Implementation of auxiliary graph
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "layout_iface.h"

/**
 * Default constructor
 */
AuxGraph::AuxGraph(): levels() 
{

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
 * Constructor from a main graph
 */
AuxGraph::AuxGraph( GraphView *graph_p): levels(), main_graph( graph_p) 
{
    initLevels( main_graph->maxRank());

    for ( NodeItem* n_item = main_graph->firstNode();
          isNotNullP( n_item);
          n_item = n_item->nextNode())
    {
        Rank rank = n_item->number( main_graph->ranks());
        AuxNode *node = newNode();
        node->setOrig( n_item);
        levels[ rank]->add( node);
    }
}

/**
 * Destructor. Cleans up level info
 */
AuxGraph::~AuxGraph()
{
    deleteLevels();
}