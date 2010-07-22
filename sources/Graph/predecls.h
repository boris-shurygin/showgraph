/**
 * @file: predecls.h
 * Predeclarations for interface of Graph library
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <QDomDocument>
#include <QtGlobal>
#include <QtGui>
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
    /** Upward direction */
	GRAPH_DIR_UP,
	/** Downward direction */
    GRAPH_DIR_DOWN,
	/** Number of directions in graph */
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
typedef quint32 GraphNum;
/** Node/edge ID type */
typedef quint64 GraphUid;

/** Maximum number of nodes */
const GraphNum GRAPH_MAX_NODE_NUM = ( GraphNum)( -1);
/** Maximum number of edges */
const GraphNum GRAPH_MAX_EDGE_NUM = ( GraphNum)( -1);

class Graph;
class Node;
class Edge;

#include "marker.h"
#include "num.h"

#endif 