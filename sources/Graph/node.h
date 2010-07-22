/**
 * @file: node.h 
 * Node class definition
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_H
#define NODE_H

/**
 * Representation of graph node. 
 *
 * @ingroup Graph
 * A graph node has two lists of edges which represent predecessors and successors.
 * It is also a member of node list in graph.
 */
class Node: public Marked, public Numbered
{
public:
    /** Node list item type */
	typedef ListItem< Node> NodeListIt;
    /** Edge list item type */
	typedef ListItem< Edge> EdgeListIt;
private:
    /** Representation in document */
    QDomElement element;

    /** Connection with inclusive graph */
    GraphUid uid; // Unique id
    Graph * graph_p;// Pointer to graph
    NodeListIt graph_it;//Item of graph's list
protected:    
    /** First edges in graph's directions */
    Edge *first_edge[ GRAPH_DIRS_NUM];

    /** Return pointer to graph's list item */
	NodeListIt* GetGraphIt()
    {
        return &graph_it;
    }
    /** We can't create nodes separately, do it through newNode method of graph */
    Node( Graph *_graph_p, GraphUid _id):uid(_id), graph_p( _graph_p), graph_it(), element()
    {
        first_edge[ GRAPH_DIR_UP] = NULL;
        first_edge[ GRAPH_DIR_DOWN] = NULL;
        graph_it.setData( ( Node*)this);
    }
	/**
	 * Detach myself from graph's node list
	 */
    inline void detachFromGraph()
    {
        graph_it.Detach();
    }
	/**
	 * Graph class controls nodes
	 */
    friend class Graph;

public:
    /**
     * Destructor
     */
    virtual ~Node();
    
    /**
	 * Return corresponding element
	 */
	inline QDomElement elem() const
    {
        return element;
    }
    /**
	 * Set element
	 */
    inline void setElement( QDomElement elem)
    {
        element = elem;
    }

    /**
     * Get node's unique ID
     */
    inline GraphUid id() const
    {
        return uid;
    }

    /**
     * Get node's corresponding graph
     */
    inline Graph * graph() const
    {
        return graph_p;
    }
    /**
	 * Next node in graph's list
	 */
    inline Node* nextNode()
    {
        return ( graph_it.next() != NULL )? graph_it.next()->data() : NULL;
    }
    
    /**
	 * Prev node in graph's list
	 */
    inline Node* prevNode()
    {
        return ( graph_it.prev() != NULL )? graph_it.prev()->data() : NULL;
    }

    /**
     * Add edge to node in specified direction
     */
    void AddEdgeInDir( Edge *edge, GraphDir dir);

    /**
     * Add predecessor edge
     */
    inline void AddPred( Edge *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }

    /**
     * Add successor edge
     */
    inline void AddSucc( Edge *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    /**
     * First edge in given direction
	 */
    inline Edge* firstEdgeInDir( GraphDir dir)
    {
        return first_edge[ dir];
    }
    /** 
     * First successor edge
	 */
    inline Edge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** 
     * First predecessor edge
	 */
    inline Edge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    
    /**
     * Deletion of edge in specified direction
     */
    void deleteEdgeInDir( GraphDir dir, Edge* edge);
    
    /**
     * delete predecessor edge
     */
    inline void deletePred( Edge* edge)
    {
        deleteEdgeInDir( GRAPH_DIR_UP, edge);
    }
    
    /**
     * delete successor edge
     */
    inline void deleteSucc( Edge* edge)
    {
        deleteEdgeInDir( GRAPH_DIR_DOWN, edge);
    }

    /**
     * Print node in DOT format to stdout
     */
    virtual void debugPrint();

    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);
};

/**
 * Add an edge to this node in specified direction
 */
inline void
Node::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    assert( isNotNullP( edge));
    EdgeListIt *it = edge->GetNodeIt( RevDir( dir));
    if ( isNotNullP( first_edge[ dir]))
    {
        it->Attach( first_edge[ dir]->GetNodeIt( RevDir( dir)));
    }
    first_edge[ dir] = edge;
}

/**
 * delete edge pointed by iterator in specidied direction
 */
inline void
Node::deleteEdgeInDir( GraphDir dir, Edge* edge)
{
    assert( isNotNullP( edge));
    if( first_edge[ dir] == edge)
    {
        first_edge[ dir] = edge->nextEdgeInDir( dir);
    }
}

/**
 * Update DOM tree element
 */
inline void
Node::updateElement()
{
    element.setAttribute( "id", id());
}

/**
 * read properties from DOM tree element
 */
inline void
Node::readFromElement( QDomElement e)
{
    element = e;
}

#endif
