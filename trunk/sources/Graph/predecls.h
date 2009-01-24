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

#endif 