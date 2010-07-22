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
 *  Representation of graph edge
 *
 *  @ingroup Graph  
 *  Edge class implements basic concept of graph edge.
 *  It has two Nodes as its end points. As edge is member of 3 lists it
 *  has 3 corresponding pointers to items of these lists
 */
class Edge: public Marked, public Numbered
{
public:
	/** Edge list item type */
    typedef ListItem< Edge> EdgeListIt;
private:
    /** Representation in document */
    QDomElement element;

    /** Graph part */
    GraphUid uid; //Unique ID
    Graph * graph_p; //Graph
    EdgeListIt graph_it; //Position in Graph's list of edges

    /** Nodes */
    Node *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    EdgeListIt n_it[ GRAPH_DIRS_NUM];//Position in each node's list

protected:
    /** Graph should have access to Edge's members */
    friend class Graph;
    /** Node should have access to Edge's members */
	friend class Node;

    /** Constructors are made private, only nodes and graph can create edges */
    Edge( Graph *_graph_p, GraphUid _id, Node *_pred, Node* _succ): uid(_id), graph_p(_graph_p), graph_it()
    {
        graph_it.setData( (Edge*) this);
        n_it[ GRAPH_DIR_UP] = EdgeListIt();
        n_it[ GRAPH_DIR_DOWN] = EdgeListIt();
        n_it[ GRAPH_DIR_UP].setData( (Edge*)this);
        n_it[ GRAPH_DIR_DOWN].setData( (Edge*)this);
        setPred( _pred);
        setSucc( _succ);
    }
    /** Pivate routines dealing with iterators */
    void SetGraphIt( EdgeListIt g_it)
    {
        graph_it = g_it;
    }
    /** 
     * Return iterator pointing to this edge in graph's edge list
     */
    EdgeListIt *GetGraphIt()
    {
        return &graph_it;
    }

    /** 
     * Set iterator pointing to this edge in graph's edge list
     */
    void SetListIt( GraphDir dir, EdgeListIt it)
    {
        n_it[ dir] = it;
    }

    /** 
     * Return iterator pointing to this edge in node's edge
     * list in corresponding direction
     */
    EdgeListIt *GetNodeIt( GraphDir dir)
    {
        return &n_it[ dir];
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
        graph_it.Detach();
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
     *      Edge * edge = graph.newEdge();
     *  
     *      //Typical deletion of edge is done by consequent calls of
     *      edge->DetachFromNode( GRAPH_DIR_UP);
     *      edge->DetachFromNode( GRAPH_DIR_DOWN);
     *      delete edge;
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
        return ( graph_it.next() != NULL )? graph_it.next()->data() : NULL;
    }

    /**
     * Return next edge of the same node in given direction
     */
    inline Edge* nextEdgeInDir( GraphDir dir)
    {
        GraphDir rdir = RevDir( dir);
        return ( n_it[ rdir].next() != NULL )? n_it[ rdir].next()->data() : NULL;
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
