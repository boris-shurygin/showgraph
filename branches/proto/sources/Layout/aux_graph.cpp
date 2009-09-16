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
 * Constructor from a main graph
 */
AuxGraph::AuxGraph( GraphView *graph_p): levels(), main_graph( graph_p) 
{
    initLevels( main_graph->maxRank());
    order = newNum();
    Numeration ranks = main_graph->ranks();
    /** Create nodes from drawable prototypes */
    for ( NodeItem* n_item = main_graph->firstNode();
          isNotNullP( n_item);
          n_item = n_item->nextNode())
    {
        Rank rank = n_item->number( ranks);
        if ( rank == NUMBER_NO_NUM)
        {
            rank = 0;
        } 
        AuxNode *node = newNode();
        node->setNode( n_item);
        node->setY( n_item->y());
        n_item->setAuxNode( node);
        node->setWidth( n_item->boundingRect().width());
        node->setHeight( n_item->boundingRect().height());
        levels[ rank]->add( node);
    }
    /** Create edges and additional nodes for representation of edge controls */
    for ( EdgeItem* e_item = main_graph->firstEdge();
          isNotNullP( e_item);
          e_item = e_item->nextEdge())
    {
        NodeItem* pred;
        NodeItem* succ;
        
        if ( e_item->pred() == e_item->succ())
            continue;

        if ( e_item->isInverted())
        {
            pred = e_item->succ();
            succ = e_item->pred();
        } else
        {
            pred = e_item->pred();
            succ = e_item->succ();
        }

        Rank pred_rank = pred->number( ranks);
        Rank succ_rank = succ->number( ranks);
        if ( pred_rank == NUMBER_NO_NUM)
        {
            pred_rank = 0;
        } 
        if ( succ_rank == NUMBER_NO_NUM)
        {
            succ_rank = pred_rank + 1;
        } 
        Rank curr_rank = pred_rank + 1;
        AuxNode *pred_node = pred->auxNode();
        AuxNode *succ_node = succ->auxNode();
        qreal y = pred->y() + RANK_SPACING;
        while ( curr_rank != succ_rank)
        {
            AuxNode *node = newNode();
            newEdge( pred_node, node);
            node->setType( AUX_EDGE_CONTROL);
            node->setEdge( e_item);
            node->setY( y);
            node->setWidth( EDGE_CONTROL_WIDTH);
            node->setHeight( EDGE_CONTROL_HEIGHT);
            levels[ curr_rank]->add( node);
            curr_rank++;
            pred_node = node;
            y+=RANK_SPACING;
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