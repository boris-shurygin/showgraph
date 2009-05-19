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

    Numeration ranks = main_graph->ranks();
    /** Create nodes from drawable prototypes */
    for ( NodeItem* n_item = main_graph->firstNode();
          isNotNullP( n_item);
          n_item = n_item->nextNode())
    {
        Rank rank = n_item->number( ranks);
        AuxNode *node = newNode();
        node->setNode( n_item);
        n_item->setAuxNode( node);
        levels[ rank]->add( node);
    }
    /** Create edges and additional nodes for representation of edge controls */
    for ( EdgeItem* e_item = main_graph->firstEdge();
          isNotNullP( e_item);
          e_item = e_item->nextEdge())
    {
        NodeItem* pred = e_item->pred();
        NodeItem* succ = e_item->succ();
        Rank pred_rank = pred->number( ranks);
        Rank succ_rank = succ->number( ranks);
        Rank curr_rank = pred_rank + 1;
        AuxNode *pred_node = pred->auxNode();
        AuxNode *succ_node = succ->auxNode();
        while ( curr_rank != succ_rank)
        {
            AuxNode *node = newNode();
            newEdge( pred_node, node);
            node->setType( AUX_EDGE_CONTROL);
            node->setEdge( e_item);

            levels[ curr_rank]->add( node);
            curr_rank++;
            pred_node = node;
        }
        newEdge( pred_node, succ_node);
    }
    debugPrint();
}

/**
 * Destructor. Cleans up level info
 */
AuxGraph::~AuxGraph()
{
    deleteLevels();
}