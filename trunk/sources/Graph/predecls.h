/**
 * File: Graph/predecls.h - Predeclarations for interface of Graph library,
 * an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <QList>
#include <QLinkedList>
#include <QVector>
#include <list>
using namespace std;

#include "../Utils/utils_iface.h"

/** 
 * namespaces import
 */
using namespace Utils;

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
 * Return direction that is reverse to given one
 */
inline GraphDir
RevDir( GraphDir dir)
{
    assert( GRAPH_DIRS_NUM == 2);
    return ( dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN: GRAPH_DIR_UP; 
}

/** Number type used for numbering nodes and edges in graph */
typedef unsigned int GraphNum;

const GraphNum GRAPH_MAX_NODE_NUM = ( GraphNum)( -1);
const GraphNum GRAPH_MAX_EDGE_NUM = ( GraphNum)( -1);

template <class Graph, class Node, class Edge> class GraphT;
template <class Graph, class Node, class Edge> class NodeT;
template <class Graph, class Node, class Edge> class EdgeT;

#include "marker.h"
#include "num.h"

#endif 