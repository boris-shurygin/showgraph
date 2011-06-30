/**
 * @file: node.h 
 * Node class definition
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef NODE_H
#define NODE_H

#include "node_iter.h"

/**
 * Representation of graph node. 
 *
 * @ingroup Graph
 * A graph node has two lists of edges which represent predecessors and successors.
 * It is also a member of node list in graph.
 */
class Node: public Marked, public Numbered, public PoolObj, public SListIface< Node, SListItem>
{
public:
    virtual ~Node();/** Destructor */
    
    inline QDomElement elem() const;           /** Return corresponding element */
	inline void setElement( QDomElement elem); /** Set element                  */

    inline GraphUid id() const;  /** Get node's unique ID           */
    inline Graph * graph() const;/** Get node's corresponding graph */
    inline Node* nextNode();     /** Next node in graph's list      */
    inline Node* prevNode();     /** Prev node in graph's list      */
    
    /** Add edge to node in specified direction */
    inline void AddEdgeInDir( Edge *edge, GraphDir dir);
    inline void AddPred( Edge *edge); /** Add predecessor edge */
    inline void AddSucc( Edge *edge); /** Add successor edge   */
 
    /** First edge in given direction */
    inline Edge* firstEdgeInDir( GraphDir dir);
    inline Edge* firstSucc(); /** First successor edge    */
    inline Edge* firstPred(); /** First predecessor edge  */
     
    /** Deletion of edge in specified direction */
    void deleteEdgeInDir( GraphDir dir, Edge* edge);
    inline void deletePred( Edge* edge); /** Delete predecessor edge */
    inline void deleteSucc( Edge* edge); /** Delete successor edge   */
    
    virtual void debugPrint(); /** Print node in DOT format to stdout */

    virtual void updateElement();                    /** Update DOM element       */ 
    virtual void readFromElement( QDomElement elem); /** Read properties from XML */

    /* Iterator types */
    typedef EdgeIterIface< SuccIterImpl> Succ;      /** Iterator for successors       */
    typedef EdgeIterIface< PredIterImpl> Pred;      /** Iterator for predecessors     */
    typedef EdgeIterIface< UnDirIterImpl> EdgeIter; /** Undirected iterator for edges */
     
    inline Succ succsBegin(); /** Create iterator for first succ */
    inline Succ succsEnd();   /** Create iterator pointing to succ end */
    
    inline Pred predsBegin(); /** Create iterator for first succ */
    inline Pred predsEnd();   /** Create iterator pointing to succ end */

    inline EdgeIter edgesBegin(); /** Create iterator for first succ */
    inline EdgeIter edgesEnd();   /** Create iterator pointing to succ end */
    

protected:
    /** We can't create nodes separately, do it through newNode method of graph */
    inline Node( Graph *_graph_p, GraphUid _id);
private:
	/** Graph class controls nodes */
    friend class Graph;	
    
    /** Detach this node from graph's node list */
    inline void detachFromGraph();

    /** Representation in document */
    QDomElement element;

    /* Connection with inclusive graph */
    GraphUid uid; /** Unique id */
    Graph * graph_p;/** Pointer to graph */

    /** First edges in graph's directions */
    Edge *first_edge[ GRAPH_DIRS_NUM];
};

#endif
