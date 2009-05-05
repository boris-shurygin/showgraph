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
 * Constructor with definition of main graph
 */
AuxGraph::AuxGraph( GraphView *graph_p): levels(), main_graph( graph_p) 
{

}