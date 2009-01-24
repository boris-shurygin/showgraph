#ifndef GRAPH_IFACE_H
#define GRAPH_IFACE_H
#include "predecls.h"

enum GraphDir
{
    GRAPH_DIR_UP,
    GRAPH_DIR_DOWN,
    GRAPH_DIRS_NUM
};

typedef list<Edge*> EdgeList;
typedef EdgeList::iterator EdgeListIt;
typedef list<Node *> NodeList;
typedef NodeList::iterator NodeListIt;

#include "graph.h"
#include "node.h"
#include "edge.h"


#endif