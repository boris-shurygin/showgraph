/**
 * File: graph.cpp - Graph class implementation, part of
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

/**
 * Constructor.
 */
template <class Graph, class Node, class Edge> 
GraphT< Graph, Node, Edge>::GraphT()
{
    node_next_id = 0;
    edge_next_id = 0;
    node_num = 0;
    edge_num = 0;
    nodes = NULL;
    edges = NULL;
    n_it = NULL;
    e_it = NULL;
    QDomElement root = createElement("graph");
    appendChild( root);
}

/** Node/Edge creation routines can be overloaded by derived class */
template <class Graph, class Node, class Edge>
void * 
GraphT< Graph, Node, Edge>::CreateNode( Graph *graph_p, int _id)
{
    return new Node ( graph_p, _id);
}

template <class Graph, class Node, class Edge>
void * 
GraphT< Graph, Node, Edge>::CreateEdge( Graph *graph_p, int _id, Node *_pred, Node* _succ)
{
    return new Edge( graph_p, _id, _pred, _succ);
}

/**
 * Creation node in graph
 */
template <class Graph, class Node, class Edge>
Node * 
GraphT< Graph, Node, Edge>::NewNode()
{
    /**
     * Check that we have available node id 
     */
    assert( edge_next_id < GRAPH_MAX_NODE_NUM);
    NodeListIt* it;
    Node *node_p = ( Node *) CreateNode( (Graph *)this, node_next_id++);
    it = node_p->GetGraphIt();
    it->Attach( nodes);
    nodes = it;
    node_num++;
    node_p->setElement( createElement( "node"));
    documentElement().appendChild( node_p->elem());
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
template <class Graph, class Node, class Edge>
Edge * 
GraphT< Graph, Node, Edge>::NewEdge( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    assert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = ( Edge *) CreateEdge( (Graph *)this, edge_next_id++, pred, succ);
    EdgeListIt* it = edge_p->GetGraphIt();
    it->Attach( edges);
    edges = it;
    edge_num++;
    edge_p->setElement( createElement( "edge"));
    documentElement().appendChild( edge_p->elem());
    return edge_p;
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
template <class Graph, class Node, class Edge>
void 
GraphT< Graph, Node, Edge>::DebugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        n->DebugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        e->DebugPrint();
    }
    out( "}");
}

/**
 * Implementation for numerations cleanup
 */
template <class Graph, class Node, class Edge>
void 
GraphT< Graph, Node, Edge>::clearNumerationsInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
}

/**
 * Implementation for markers cleanup
 */
template <class Graph, class Node, class Edge>
void 
GraphT< Graph, Node, Edge>::clearMarkersInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); !endOfNodes(); n = nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}

/**
 * Implementation for markers cleanup
 */
template <class Graph, class Node, class Edge>
void 
GraphT< Graph, Node, Edge>::writeToXML()
{
    QString fileName( "test.xml");
    QFile file( fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        assert( 0);
        return;
    }
     
    /** Clean markers in nodes */
    for ( Node *n = firstNode(); !endOfNodes(); n = nextNode())
    {
        n->updateElement();
    }

    /** Clean markers in edges */
    for (  Edge *e = firstEdge(); !endOfEdges(); e = nextEdge())
    {
        e->updateElement();
    }

    const int IndentSize = 4;

    QTextStream out( &file);
    save(out, IndentSize);
}