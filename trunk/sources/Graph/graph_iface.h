/**
 * File: graph_iface.h - Interface of Graph library,
 * an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_IFACE_H
#define GRAPH_IFACE_H
#include "predecls.h"

/**
 * Directions type in graph
 */
enum GraphDir
{
    GRAPH_DIR_UP,
    GRAPH_DIR_DOWN,
    GRAPH_DIRS_NUM
};

/**
 * Lists of nodes and edges
 */
typedef list<Edge*> EdgeList;
typedef EdgeList::iterator EdgeListIt;
typedef list<Node *> NodeList;
typedef NodeList::iterator NodeListIt;

#include "graph.h"
#include "node.h"
#include "edge.h"


#endif