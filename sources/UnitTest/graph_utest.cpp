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
    ANode *pred = graph.NewNode();
    ANode *succ = graph.NewNode();
    AEdge *edge = graph.NewEdge( pred, succ);
    Marker m = graph.NewMarker();
    Marker m2 = graph.NewMarker();

    Marker m_array[ MAX_GRAPH_MARKERS];
    
    Assert( !pred->IsMarked( m));
    Assert( !succ->IsMarked( m));
    Assert( !edge->IsMarked( m));
    Assert( !pred->IsMarked( m2));
    
    pred->Mark( m);
    succ->Mark( m);
    edge->Mark( m);
    edge->Mark( m2);

    Assert( pred->IsMarked( m));
    Assert( succ->IsMarked( m));
    Assert( edge->IsMarked( m));
    Assert( edge->IsMarked( m2));
    edge->Unmark( m);
    Assert( edge->IsMarked( m2));
    Assert( !edge->IsMarked( m));
    
    graph.FreeMarker( m);
    graph.FreeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        m_array [ i] = graph.NewMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        graph.FreeMarker( m_array[ i]);
    }
    m = graph.NewMarker();
    graph.FreeMarker( m);
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
    //Assert<Error>( 0);
    return true;
}