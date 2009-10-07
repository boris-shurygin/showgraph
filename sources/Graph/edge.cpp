/**
 * @file: edge.cpp 
 * Edge class implementation
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

/**
 * Edge destructor.
 * delete edge from graph's list of edges
 */
template <class Graph, class Node, class Edge>
EdgeT<Graph, Node, Edge>::~EdgeT()
{
    //out("Deleted edge");
    element.parentNode().removeChild( element);
    graph_p->deleteEdge( ( Edge *)this);
    detachFromNode( GRAPH_DIR_UP);
    detachFromNode( GRAPH_DIR_DOWN);
}

/**
 * Print edge in DOT format to stdout
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::debugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
    assert( isNotNullP( pred()));
    assert( isNotNullP( succ()));

    out("%llu->%llu;", pred()->id(), succ()->id());
}

/**
 * Low level correction of node's edge list in corresponding direction
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::detachFromNode( GraphDir dir)
{
    if ( isNotNullP( node( dir)))
    {
        Node *n = node( dir);
        n->deleteEdgeInDir( RevDir( dir), (Edge* )this);
        n_it[ dir].Detach();
        nodes[ dir] = 0;
    }
}

/**
 * Update DOM tree element
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::updateElement()
{
    element.setAttribute( "source", pred()->id());
    element.setAttribute( "target", succ()->id());
}

/**
 * read properties from DOM tree element
 */
template <class Graph, class Node, class Edge>
void
EdgeT<Graph, Node, Edge>::readFromElement( QDomElement e)
{
    element = e;
}