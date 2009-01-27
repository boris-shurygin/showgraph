/**
 * File: Graph/predecls.h - Predeclarations for interface of Graph library,
 * an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <list>
using namespace std;
class Graph;
class Node;
class Edge;
class GraphError;

/**
 * Graph's assertion routines;
 */
void GraphAssert( bool asrt);
void GraphAssert( bool asrt, GraphError e);

/** Number type used for numbering nodes and edges in graph */
typedef unsigned int GraphNum;

#define GRAPH_MAX_NODE_NUM ( GraphNum)( -1)
#define GRAPH_MAX_EDGE_NUM ( GraphNum)( -1)

#endif 