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
    first_node = NULL;
    first_edge = NULL;
    QDomElement root = createElement("graph");
    appendChild( root);
}

/**
 * Build graph from XML description
 */
template <class Graph, class Node, class Edge> 
void
GraphT< Graph, Node, Edge>::readFromXML( QString filename)
{
    QFile file( filename);

    if ( !file.open( QIODevice::ReadOnly))
        return;

    if ( !setContent( &file))
    {
        file.close();
        return;
    }
    file.close();

    /**
     * Read nodes and create them
     */
    QDomElement docElem = documentElement();

    QDomNode n = docElem.firstChild();
    QHash< GraphUid, Node *> n_hash;

    while ( !n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        
        if ( !e.isNull() && e.tagName() == QString( "node"))
        {
            Node *node = newNode( e);
            node->readFromElement( e);
            n_hash[ e.attribute( "id").toLongLong()] = node;
        }
        n = n.nextSibling();
    }
    
    n = docElem.firstChild();
    while ( !n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the DOM tree node to an element.
        
        if ( !e.isNull() && e.tagName() == QString( "edge"))
        {
            GraphUid pred_id = e.attribute( "source").toLongLong();
            GraphUid succ_id = e.attribute( "target").toLongLong();
            Node *pred = n_hash[ pred_id];
            Node *succ = n_hash[ succ_id];
            Edge *edge = newEdge( pred, succ, e);
            edge->readFromElement( e);
        }
        n = n.nextSibling();
    }
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
GraphT< Graph, Node, Edge>::newNode()
{
    Node * node_p = newNodeImpl( node_next_id);
    node_p->setElement( createElement( "node"));
    documentElement().appendChild( node_p->elem());
    return node_p;
}

/**
 * Creation node in graph
 */
template <class Graph, class Node, class Edge>
Node * 
GraphT< Graph, Node, Edge>::newNodeImpl( GraphUid id)
{
    /**
     * Check that we have available node id 
     */
    assert( edge_next_id < GRAPH_MAX_NODE_NUM);
    
    Node *node_p = ( Node *) CreateNode( (Graph *)this, id);
    NodeListIt* it = node_p->GetGraphIt();
    
    /** Add node to graph's list of nodes */
    if ( IsNotNullP( first_node))
    {
        it->Attach( first_node->GetGraphIt());
    }
    first_node = node_p;
    
    node_num++;
    
    /** Make sure that next automatically assigned id will be greater than given id */
    if ( node_next_id <= id)
        node_next_id = id + 1;
    return node_p;
}

/**
 * Creation node in graph
 */
template <class Graph, class Node, class Edge>
Node * 
GraphT< Graph, Node, Edge>::newNode( QDomElement e)
{
    assert( !e.isNull());
    assert( e.tagName() == QString( "node"));
    assert( e.hasAttribute( "id"));

    Node *node_p = newNodeImpl( node_next_id);
    node_p->setElement( e);
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
template <class Graph, class Node, class Edge>
Edge * 
GraphT< Graph, Node, Edge>::newEdgeImpl( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    assert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = ( Edge *) CreateEdge( (Graph *)this, edge_next_id++, pred, succ);
    EdgeListIt* it = edge_p->GetGraphIt();
    if ( IsNotNullP( first_edge))
    {
        it->Attach( first_edge->GetGraphIt());
    }
    first_edge = edge_p;
    edge_num++;
    return edge_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
template <class Graph, class Node, class Edge>
Edge * 
GraphT< Graph, Node, Edge>::newEdge( Node * pred, Node * succ)
{
    Edge *edge_p = newEdgeImpl( pred, succ);
    edge_p->setElement( createElement( "edge"));
    documentElement().appendChild( edge_p->elem());
    return edge_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
template <class Graph, class Node, class Edge>
Edge * 
GraphT< Graph, Node, Edge>::newEdge( Node * pred, Node * succ, QDomElement e)
{
    Edge *edge_p = newEdgeImpl( pred, succ);
    edge_p->setElement( e);
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
    for (  n = firstNode(); IsNotNullP( n); n = n->nextNode())
    {
        n->DebugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); IsNotNullP( e); e = e->nextEdge())
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
    for (  n = firstNode(); IsNotNullP( n); n = n->nextNode())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); IsNotNullP( e); e = e->nextEdge())
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
    for (  n = firstNode(); IsNotNullP( n); n = n->nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); IsNotNullP( e); e = e->nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}

/**
 * Implementation for markers cleanup
 */
template <class Graph, class Node, class Edge>
void 
GraphT< Graph, Node, Edge>::writeToXML( QString filename)
{
    QFile file( filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        assert( 0);
        return;
    }
     
    /** Clean markers in nodes */
    for ( Node *n = firstNode(); IsNotNullP( n); n = n->nextNode())
    {
        n->updateElement();
    }

    /** Clean markers in edges */
    for ( Edge *e = firstEdge(); IsNotNullP( e); e = e->nextEdge())
    {
        e->updateElement();
    }

    const int IndentSize = 4;

    QTextStream out( &file);
    save(out, IndentSize);
}