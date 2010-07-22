/**
 * @file: graph.cpp
 * Graph class implementation
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * Constructor.
 */
Graph::Graph()
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
 * Destructor - removes all nodes
 */
Graph::~Graph()
{
    for ( Node *node = firstNode();
          isNotNullP( node);
          )
    {
        Node* next = node->nextNode();
        delete node;
        node = next;
    }
}

/**
 * Build graph from XML description
 */
void
Graph::readFromXML( QString filename)
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
Node * 
Graph::CreateNode( int _id)
{
    return new Node ( this, _id);
}

Edge * 
Graph::CreateEdge( int _id, Node *_pred, Node* _succ)
{
    return new Edge( this, _id, _pred, _succ);
}
/**
 * Creation node in graph implementation
 */
inline Node * 
Graph::newNodeImpl( GraphUid id)
{
    /**
     * Check that we have available node id 
     */
    assert( edge_next_id < GRAPH_MAX_NODE_NUM);
    
    /** Create node */
    Node *node_p = CreateNode( id);
    NodeListIt* it = node_p->GetGraphIt();
    
    /** Add node to graph's list of nodes */
    if ( isNotNullP( first_node))
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
Node * 
Graph::newNode()
{
    Node * node_p = newNodeImpl( node_next_id);
    node_p->setElement( createElement( "node"));
    documentElement().appendChild( node_p->elem());
    return node_p;
}


/**
 * Creation node in graph
 */
Node * 
Graph::newNode( QDomElement e)
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
inline Edge * 
Graph::newEdgeImpl( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    assert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = CreateEdge( edge_next_id++, pred, succ);
    EdgeListIt* it = edge_p->GetGraphIt();
    if ( isNotNullP( first_edge))
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
Edge * 
Graph::newEdge( Node * pred, Node * succ)
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
Edge * 
Graph::newEdge( Node * pred, Node * succ, QDomElement e)
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
void 
Graph::debugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->debugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->debugPrint();
    }
    out( "}");
}

/**
 * Implementation for numerations cleanup
 */
void 
Graph::clearNumerationsInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
}

/**
 * Implementation for markers cleanup
 */
void 
Graph::clearMarkersInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}

/**
 * Implementation for markers cleanup
 */
void 
Graph::writeToXML( QString filename)
{
    QFile file( filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        assert( 0);
        return;
    }
     
    /** Clean markers in nodes */
    for ( Node *n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->updateElement();
    }

    /** Clean markers in edges */
    for ( Edge *e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->updateElement();
    }

    const int IndentSize = 4;

    QTextStream out( &file);
    save(out, IndentSize);
}