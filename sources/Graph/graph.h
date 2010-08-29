/**
 * @file: graph.h 
 * Graph class definition/implementation.
 *
 * @defgroup Graph Graph representation
 *
 * Graph representation is implemented through 3 tightly connected classes Graph, Node and Edge.
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H
/**
 *  Graph representation  
 *
 *  @ingroup Graph
 *  Like classical definition G = ( N, E) where N is set of nodes n and E is set of edges e = {n_i, n_j}
 *  A graph has pointers to its first edge and node which allows iteration through them using nextEdge
 *  and nextNode methods of Node and Edge classes
 */
class Graph: public MarkerManager, public NumManager, public QDomDocument
{
public:
    /** Node list item type */
	typedef ListItem< Node> NodeListIt;
	/** Edge list item type */
	typedef ListItem< Edge> EdgeListIt;
private:
    /** First node */
    Node* first_node;
    /** Number of nodes */
    GraphNum node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphUid node_next_id;

    /* List of edges and its iterator */
    Edge* first_edge;
    GraphNum edge_num;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphUid edge_next_id;



    /**
     * Implementation of node creation
     */
    Node * newNodeImpl( GraphUid id);
    /**
     * Implementation of edge creation
     */
	Edge * newEdgeImpl( Node * pred, Node * succ);
protected:
    /**
     * Pool for nodes
     */
    Pool *node_pool;

    /**
     * Pool for edges
     */
    Pool *edge_pool;

    /** Node creation routine is to be overloaded by derived class */
	virtual Node * createNode( int _id);
	/** Edge creation routine is to be overloaded by derived class */
    virtual Edge * createEdge( int _id, Node *_pred, Node* _succ);
    


    /** Pools' creation routine */
    virtual void createPools();
    /** Pools' destruction routine */
    virtual void destroyPools();

    /** Get pool of nodes */
    inline Pool *nodePool() const
    {
        return node_pool;
    }
    /** Get pool of edges */
    inline Pool *edgePool() const
    {
        return edge_pool;
    }
public:
    /**
     * Constructor. 
     * From inherited classes it may be called with 'false'
     * vaule of the parameter to prevent pools creation
     */
    Graph( bool create_pools);
    
    /** Destructor */
    virtual ~Graph();

    /** Create new node in graph */
    Node * newNode();

    /** Create new node in graph and fills it with info in element */
    Node * newNode( QDomElement e);

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    Edge * newEdge( Node * pred, Node * succ);
    /**
     * Create edge between two nodes from an XML description
     * We do not support creation of edge with undefined endpoints
     */
	Edge * newEdge( Node * pred, Node * succ, QDomElement e);
    
    /** Node deletion routine is to be overloaded by derived class */
    inline void deleteNode( void *n)
    {
        node_pool->destroy( n);
    }

    /** Edge deletion routine is to be overloaded by derived class */
    inline void deleteEdge( void *e)
    {
        edge_pool->destroy( e);
    }
    
    /**
     * Remove node from node list of graph
     */
    inline void detachNode( Node* node)
    {
        assert( isNotNullP( node));
        assert( node->graph() == this);

        if( first_node == node)
        {
           first_node = node->nextNode();
        }
        node->detachFromGraph();
        node_num--;
    }

    /**
     * Remove edge from edge list of graph
     */
    inline void detachEdge( Edge * edge)
    {
        assert( isNotNullP( edge));
        assert( edge->graph() == this);

        if( first_edge == edge)
        {
           first_edge = edge->nextEdge();
        }
        edge->detachFromGraph();
        edge_num--;
    }

    /**
     * Return node quantity
     */
    inline GraphNum nodeCount() const
    {
        return node_num;
    }

    /**
     * Return edge quantity
     */
    inline GraphNum edgeCount() const
    {
        return edge_num;
    }
    /** 
     * Get first edge
     */
    inline Edge* firstEdge() 
    {
        return first_edge;
    }
    
    /**
	 * Get first node
	 */
    inline Node* firstNode()
    {
        return first_node;
    }
    /**
     * Print graph to stdout in DOT format
     */
    virtual void debugPrint();
    
    /**
     * Clear unused markers from marked objects
     */
    void clearMarkersInObjects();

    /**
     * Clear unused numerations from numbered objects
     */
    void clearNumerationsInObjects();

    /**
     * Save graph as an XML file
     */
    void writeToXML( QString filename);

    /**
     * Build graph from XML description
     */
    void readFromXML( QString filename);
};

#endif
