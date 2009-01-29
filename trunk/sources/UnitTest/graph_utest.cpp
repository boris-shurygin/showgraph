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

    /** Nodes traversal */
    //Assert<Error>( 0);
    return true;
}