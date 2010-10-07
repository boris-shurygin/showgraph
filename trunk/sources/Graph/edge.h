/**
 * @file: edge.h
 * Edge class definition
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef EDGE_H
#define EDGE_H

/**
 * Edge lists identificators
 * @ingroup Graph 
 */
enum EdgeListType
{
    EDGE_LIST_PREDS,
    EDGE_LIST_SUCCS,
    EDGE_LIST_GRAPH,
    EDGE_LISTS_NUM
};

/**
 *  Representation of graph edge
 *
 *  @ingroup Graph  
 *  Edge class implements basic concept of graph edge.
 *  It has two Nodes as its end points. Edge is a member of 3 lists: 
 *  edge list in graph, pred list in succ node and succ list in pred node
 */
class Edge: 
    public MListIface< Edge, // List item
                       MListItem< EDGE_LISTS_NUM>, // base class: pure multi-list item
                       EDGE_LISTS_NUM >, // Lists number                      
    public Marked,
    public Numbered,
    public PoolObj
{
public:
	/** Edge list item type */
    //typedef ListItem< Edge> EdgeListIt;
private:
    /** Representation in document */
    QDomElement element;

    /** Graph part */
    GraphUid uid; //Unique ID
    Graph * graph_p; //Graph

    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    /** Node checking routine */
    bool checkNodes( Node* _pred, Node* _succ);
        
protected:
    /** Graph should have access to Edge's members */
    friend class Graph;
    /** Node should have access to Edge's members */
	friend class Node;

    /** Constructors are made private, only nodes and graph can create edges */
    Edge( Graph *_graph_p, GraphUid _id, Node *_pred, Node* _succ): 
        uid(_id), graph_p(_graph_p)
    {
        GRAPH_ASSERTD( checkNodes( _pred, _succ),
                       "Predecessor and sucessor used in edge construction belong to different graphs");
        setPred( _pred);
        setSucc( _succ);
    }

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    void detachFromNode( GraphDir dir);
    /**
	 * Remove myself from graph's list of edges
	 */
	inline void detachFromGraph()
    {
        detach( EDGE_LIST_GRAPH);
    }
public:
    /**
	 * Return corresponding document element
	 */
    inline QDomElement elem() const
    {
        return element;
    }

    /**
	 * Set document element
	 */    
    inline void setElement( QDomElement elem)
    {
        element = elem;
    }

    /**
     * Get edge's unique ID
     */
    inline GraphUid id() const
    {
        return uid;
    }

    /**
     * Get edge's corresponding graph
     */
    inline Graph * graph() const
    {
        return graph_p;
    }

    /** 
     *  Destructor.
	 *  Delete edge from list in graph.
     *  Deletion from node lists MUST be performed manually.
     *  Example: 
     *      Graph graph;
     *      ...
     *      Edge * edge = graph.newEdge(...);
     *  
     *      //Typical deletion of edge is done by consequent calls of
     *      edge->DetachFromNode( GRAPH_DIR_UP);
     *      edge->DetachFromNode( GRAPH_DIR_DOWN);
     *      graph.deleteEdge( edge);
     */
    virtual ~Edge();

    /**
     * Connect edge to a node in specified direction.
     * Note that node treats this edge in opposite direction. I.e. an edge that has node in
     * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
     */
    inline void setNode( Node *n, GraphDir dir);
    
    /**
     * Connect edge with given node as a predecessor
     */
    inline void setPred( Node *n)
    {
        setNode( n, GRAPH_DIR_UP);
    }
    /**
     * Connect edge with given node as a successor
     */
    inline void setSucc( Node *n)
    {
        setNode( n, GRAPH_DIR_DOWN);
    }

    /**
     * Get node in specified direction
     */
    inline Node *node( GraphDir dir) const 
    {
        return nodes[ dir];
    }
    /**
     * Get predecessor of edge
     */
    inline Node *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /**
     * Get successor of edge
     */
    inline Node *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }

    /**
     * Return next edge of the same node in given direction
     */
    inline Edge* nextEdge()
    {
        return next( EDGE_LIST_GRAPH);
    }

    /**
     * Return next edge of the same node in given direction
     */
    inline Edge* nextEdgeInDir( GraphDir dir)
    {
        GRAPH_ASSERTD( dir < GRAPH_DIRS_NUM, "Wrong direction parameter");
        GRAPH_ASSERTD( GRAPH_DIR_DOWN == EDGE_LIST_SUCCS,
                       "Enums of direction and edge lists are not having right values");
        GRAPH_ASSERTD( GRAPH_DIR_UP == EDGE_LIST_PREDS,
                       "Enums of direction and edge lists are not having right values");
        return next( dir);
    }
    
    /** 
     * Next successor
     */
    inline Edge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
	/** 
     * Next predecessor
     */
    inline Edge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    }
    
    /**
     * Print edge in dot fomat to stdout
     */
    virtual inline void debugPrint();

    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    /**
     * Insert a node on this edge
	 *
	 * Creates a node on edge and a new edge from new node to former successor of original edge.
	 * Original edge goes to new node. 
	 * Return new node.
	 */
    inline Node *insertNode();
};

#endif
