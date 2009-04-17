/**
 * File: graph_utest.cpp - Implementation of testing of Graph library, 
 * internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"
#include <vector>

using namespace std;

 /**
  * TODO: Check graph's data structures being consistent with node and edge functionality
  */
bool UTestGraphOwn()
{
    return true;
}
/**
 * TODO: Check consistency of Node and Edge classes interoperation
 */
bool UTestNodeEdge()
{
    /**
     * TODO: Check that node and edge remain correct after basic edge/node creation/deletion
     */
    /**
     * 
     */
    return true;
}

/**
 * Check marker functionality
 */
bool UTestMarkers()
{
    AGraph graph;
    ANode *dummy = graph.NewNode();
    delete dummy;
    ANode *pred = graph.NewNode();
    ANode *succ = graph.NewNode();
    AEdge *edge = graph.NewEdge( pred, succ);
    Marker m = graph.newMarker();
    Marker m2 = graph.newMarker();

    Marker m_array[ MAX_GRAPH_MARKERS];
    
    assert( !pred->isMarked( m));
    assert( !succ->isMarked( m));
    assert( !edge->isMarked( m));
    assert( !pred->isMarked( m2));
    
    pred->mark( m);
    succ->mark( m);
    edge->mark( m);
    edge->mark( m2);

    assert( pred->isMarked( m));
    assert( succ->isMarked( m));
    assert( edge->isMarked( m));
    assert( edge->isMarked( m2));
    edge->unmark( m);

    /** Check that different markers have different behaviour */
    assert( edge->isMarked( m2));
    assert( !edge->isMarked( m));
    
    graph.freeMarker( m);
    graph.freeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        m_array [ i] = graph.newMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        graph.freeMarker( m_array[ i]);
    }
    m = graph.newMarker();
    graph.freeMarker( m);
    
    ANode *n;
    for (  n = graph.firstNode(); !graph.endOfNodes(); n = graph.nextNode())
    {
        delete n;
    }
    return true;
}

/**
 * Unit tests for Graph library
 */
bool UTestGraph()
{
    AGraph graph;

    /** 
     *  Check basic operation of graph library
     */
    vector<ANode *> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes.push_back( graph.NewNode());
        if ( i > 0)
        {
            graph.NewEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.NewEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.NewEdge( nodes[ 8], nodes[ 4]);
    delete nodes[ 8];
    graph.DebugPrint();
    graph.writeToXML();

    /**
     * Check graph's data structures consistency
     */
     if ( !UTestGraphOwn())
         return false;
    /**
     * Check node-edge consistency
     */
    if ( !UTestNodeEdge())
         return false;

    /**
     * Check markers
     */
    if ( !UTestMarkers())
        return false;

    /** Nodes traversal */
    //assert<Error>( 0);
    return true;
}