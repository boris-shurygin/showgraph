/**
 * @file: graph_iface.h
 * Interface of Graph library
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_IFACE_H
#define GRAPH_IFACE_H
#include "predecls.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "agraph.h"

/**
 * Convinience macro for traversing edges
 * parameter: node - a node that we use to get first edge
 * parameter: edge - an object of Edge type or subclass of Edge which is the loop variable
 * parameter: dir - 'Succ' or 'Pred'
 */
#define ForEdges(node, edge, dir) for ( edge = node->first##dir();\
										isNotNullP( edge);\
								        edge = edge->next##dir())
#endif