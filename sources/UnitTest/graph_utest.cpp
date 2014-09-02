/**
 * @file: graph_utest.cpp 
 * Implementation of testing of Graph library
 */
/*
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "utest_impl.h"
#include <vector>

using namespace std;

 /**
  * TODO: Check graph's data structures being consistent with node and edge functionality
  */
bool uTestGraphOwn()
{
    {
        AGraph graph( true);
        ANode *dummy = graph.newNode();
        graph.deleteNode( dummy);
        ANode *pred = graph.newNode();
        ANode *succ = graph.newNode();
        AEdge *edge = graph.newEdge( pred, succ);
        
        /** Check node insertion */
        ANode *new_node = edge->insertNode();
        AEdge *edge2 = new_node->firstSucc();
        assert( areEqP( new_node->firstPred(), pred->firstSucc())); 
        assert( areEqP( new_node->firstSucc(), succ->firstPred()));
        assert( areEqP( edge->pred(), pred));
        assert( areEqP( pred->firstSucc(), edge));
        assert( areEqP( edge->succ(), new_node));
        assert( areEqP( new_node->firstPred(), edge));
        assert( areEqP( edge2->pred(), new_node));
        assert( areEqP( edge2->succ(), succ));
        assert( areEqP( succ->firstPred(), edge2));
    }

    /** Test iterators */
    {
        AGraph graph( true);
        ANode *node1 = graph.newNode();
        ANode *node2 = graph.newNode();
        ANode *node3 = graph.newNode();
        AEdge *edge1 = graph.newEdge( node1, node2);
        AEdge *edge2 = graph.newEdge( node2, node3);
        
        for ( Node::Succ succ_iter = node2->succsBegin(),
                         succ_iter_end = node2->succsEnd();
              succ_iter != succ_iter_end;
              succ_iter++ )
        {
            assert( areEqP( *succ_iter, edge2));
        }
        for ( Node::Pred pred_iter = node2->predsBegin(),
                         pred_iter_end = node2->predsEnd();
              pred_iter != pred_iter_end;
              pred_iter++ )
        {
            assert( areEqP( *pred_iter, edge1));
        }
        Node::EdgeIter edge_iter = node2->edgesBegin();
        Node::EdgeIter edge_iter_end = node2->edgesEnd();
        assert( edge_iter != edge_iter_end);
        assert( areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            assert( areEqP( edge_iter.node(), edge1->pred()));
            assert( areEqP( edge_iter.node(), node1));
        } else
        {
            assert( areEqP( edge_iter.node(), edge2->succ()));
            assert( areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        assert( edge_iter != edge_iter_end);
        assert( areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            assert( areEqP( edge_iter.node(), edge1->pred()));
            assert( areEqP( edge_iter.node(), node1));
        } else
        {
            assert( areEqP( edge_iter.node(), edge2->succ()));
            assert( areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        assert( edge_iter == edge_iter_end);
    }

    return true;
}
/**
 * TODO: Check consistency of Node and Edge classes interoperation
 */
bool uTestNodeEdge()
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
bool uTestMarkers()
{
    AGraph graph( true);
    ANode *dummy = graph.newNode();
    graph.deleteNode( dummy);
    ANode *pred = graph.newNode();
    ANode *succ = graph.newNode();
    AEdge *edge = graph.newEdge( pred, succ);
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
    for (  n = graph.firstNode(); isNotNullP( n);)
    {
        ANode *tmp = n;
        n = n->nextNode();
        graph.deleteNode( tmp);
    }
    return true;
}

/**
 * Check marker functionality
 */
static bool uTestNumerations()
{
    /** 
     * Every class that can be a numerations manager should implement
     * the routine for clearing numerations in objects
     */
    class NumMgrInst: public NumManager
    {
        /** Implementation of clearing - empty TODO: implement and test it */
        void clearNumerationsInObjects()
        {
        
        }
    };
    /**
     * Check correct error reporting
     *  1. Too many numerations
     */
    NumMgrInst mgr1;
    try
    {
        for ( int i = 0; i < MAX_NUMERATIONS + 1; i++)
        {
            mgr1.newNum();
        }
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        assert( error == NUM_ERROR_OUT_OF_INDEXES);
    }

    /** 2. Too big number */
    NumMgrInst mgr2;
    Numeration num2 = mgr2.newNum();
    Numbered obj2; 
    try
    {
        obj2.setNumber( num2, -1);
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        assert( error == NUM_ERROR_NUMBER_OUT_OF_RANGE);
    }
    mgr2.freeNum( num2);

    /** 3. Functional testing */
    NumMgrInst mgr;
    for ( int i = 0; i < MAX_NUMERATIONS + 2; i++)
    {
        Numeration n = mgr.newNum();
        mgr.freeNum( n);
    } 
    Numeration num = mgr.newNum();
    Numeration num_unused = mgr.newNum();
    Numbered obj; 
    assert( obj.number( num) == NUMBER_NO_NUM);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.setNumber( num, 1);
    assert( obj.isNumbered( num));
    assert( obj.number( num) == 1);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.unNumber( num);
    assert( obj.number( num) == NUMBER_NO_NUM);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);    
    return true;
}

/**
 * Create graph and save it to XML
 */
bool uTestSave()
{
    AGraph graph( true);

    /** 
     *  Check basic operation of graph library
     */
    vector<ANode *> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes.push_back( graph.newNode());
        if ( i > 0)
        {
            graph.newEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.newEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.newEdge( nodes[ 8], nodes[ 4]);
    graph.deleteNode( nodes[ 8]);
    graph.debugPrint();
    graph.writeToXML( QString ( "test.xml"));
    return true;
}

/**
 * Load from XML
 */
bool uTestLoad()
{
    AGraph graph( true);
    graph.readFromXML( QString( "test.xml"));
    graph.debugPrint();
    return true;
}

/**
 * Unit tests for Graph XML save/load
 */
bool uTestXML()
{
    if ( !uTestSave())
        return false;
    
    if ( !uTestLoad())
        return false;
    return true;
}

/**
 * Unit tests for Graph library
 */
bool uTestGraph()
{
    /**
     * Check graph's data structures consistency
     */
     if ( !uTestGraphOwn())
         return false;
    /**
     * Check node-edge consistency
     */
    if ( !uTestNodeEdge())
         return false;

    /**
     * Check numerations 
     */
    if ( !uTestNumerations())
        return false;

    /**
     * Check markers
     */
    if ( !uTestMarkers())
        return false;

    /**
     * Check xml I/O
     */
    if ( !uTestXML())
        return false;

//    assert<Error>( 0);
    return true;
}