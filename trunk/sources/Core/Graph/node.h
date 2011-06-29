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

/** Baseclass for implementing iterator template parameters */
class IterImplBase
{
public:
    /** Get edge */
    inline Edge* edge() const { return edge_p;}
    /** Set edge */
    inline void setEdge( Edge *e){ edge_p = e;}
    /** Default constructor */
    inline IterImplBase(): edge_p( NULL) {}
protected:
    Edge *edge_p;
};

/** Parameter for iterator template specialization (pred traversal) */
class PredIterImpl: public IterImplBase
{
public:
    inline void nextEdge();       /** Move on to the next pred */
    inline Node *node() const;    /** Get node */
    inline PredIterImpl(){};      /** Default constructor */
    inline PredIterImpl( Node *n);/** Configures iterator with node's first pred */
    inline bool operator==(const PredIterImpl& o) const /** Comparison operator */
    { 
        return edge_p == o.edge_p;
    }
};
/** Parameter for iterator template specialization (succ traversal) */
class SuccIterImpl: public IterImplBase
{
public:
    inline void nextEdge();       /** Move on to the next succ */
    inline Node *node() const;    /** Get node */
    inline SuccIterImpl(){};      /** Default constructor */
    inline SuccIterImpl( Node *n);/** Configures iterator with node's first succ */
    inline bool operator==(const SuccIterImpl& o) const /** Comparison operator */
    { 
        return edge_p == o.edge_p;
    }
};

/** Parameter for iterator template specialization (undirected traversal) */
class UnDirIterImpl: public IterImplBase
{
public:
    inline void nextEdge();        /** Move on to the next edge */
    inline Node *node() const;     /** Get node */
    inline UnDirIterImpl():is_pred( false){};      /** Default consturctor */
    inline UnDirIterImpl( Node *n);/** Configures iterator with node's first edge */
    inline bool operator==(const UnDirIterImpl& o) const /** Comparison operator */
    { 
        return edge_p == o.edge_p 
               && is_pred == o.is_pred;
    }
private:
    bool is_pred;
};

/**
 * Convinience template for iterating through node's adjacent edges
 */
template < class EdgeIterImpl> class EdgeIterIface
{   
public:
    /** Default constructor */
    inline EdgeIterIface();
    /** Copy constructor */
    inline EdgeIterIface( const EdgeIterIface& proto);
    /** Destructor */
    inline ~EdgeIterIface();
    /** Preincrement operator */
    inline EdgeIterIface & operator++();
    /** Postincrement operator */
    inline EdgeIterIface operator++( int);
    /** Dereferenece operator*/
    inline Edge * operator*();
    /** Comparison operator */
    inline bool operator==(const EdgeIterIface& o) const; 
    /** Not equals operator */
    inline bool operator!=(const EdgeIterIface& o) const;
    /** Get Edge */
    inline Edge *edge() const;
    /** Get node on the end of edge */
    inline Node *node() const;
private:
    /** Constructor from node */
    inline EdgeIterIface( Node *e);
    friend class Node;

    /** Parameter-dependent implementation */
    EdgeIterImpl impl;
};

/**
 * Representation of graph node. 
 *
 * @ingroup Graph
 * A graph node has two lists of edges which represent predecessors and successors.
 * It is also a member of node list in graph.
 */
class Node: public Marked, public Numbered, public PoolObj, public SListIface< Node, SListItem>
{
private:
    /** Representation in document */
    QDomElement element;

    /** Connection with inclusive graph */
    GraphUid uid; // Unique id
    Graph * graph_p;// Pointer to graph

protected:    
    /** First edges in graph's directions */
    Edge *first_edge[ GRAPH_DIRS_NUM];

    /** We can't create nodes separately, do it through newNode method of graph */
    Node( Graph *_graph_p, GraphUid _id):uid(_id), graph_p( _graph_p), element()
    {
        first_edge[ GRAPH_DIR_UP] = NULL;
        first_edge[ GRAPH_DIR_DOWN] = NULL;
    }
	/**
	 * Detach myself from graph's node list
	 */
    inline void detachFromGraph()
    {
        detach();
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
        return next();
    }
    
    /**
	 * Prev node in graph's list
	 */
    inline Node* prevNode()
    {
        return prev();
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

    /**
     * Iterator for successors
     */
    typedef EdgeIterIface< SuccIterImpl> Succ;
    /**
     * Iterator for predecessors
     */
    typedef EdgeIterIface< PredIterImpl> Pred;
    /**
     * Undirected iterator for edges
     */
    typedef EdgeIterIface< UnDirIterImpl> EdgeIter;
    /**
     * Create iterator for first succ
     */
    inline Succ succsBegin();
    /**
     * Create iterator pointing to succ end
     */
    inline Succ succsEnd();
    /**
     * Create iterator for first succ
     */
    inline Pred predsBegin();
    /**
     * Create iterator pointing to succ end
     */
    inline Pred predsEnd();
        /**
     * Create iterator for first succ
     */
    inline EdgeIter edgesBegin();
    /**
     * Create iterator pointing to succ end
     */
    inline EdgeIter edgesEnd();
};

/**
 * Create iterator for first succ
 */
Node::Succ Node::succsBegin()
{
    return Succ( this);
}

/**
 * Create iterator pointing to succ end
 */
Node::Succ Node::succsEnd()
{
    return Succ();
}
/**
 * Create iterator for first succ
 */
Node::Pred Node::predsBegin()
{
    return Pred( this);
}
/**
 * Create iterator pointing to succ end
 */
Node::Pred Node::predsEnd()
{
    return Pred();
}

/**
 * Create iterator for first succ
 */
Node::EdgeIter Node::edgesBegin()
{
    return EdgeIter( this);
}
/**
 * Create iterator pointing to succ end
 */
Node::EdgeIter Node::edgesEnd()
{
    return EdgeIter();
}

/**
 * Add an edge to this node in specified direction
 */
inline void
Node::AddEdgeInDir( Edge *edge, GraphDir dir)
{
    assert( isNotNullP( edge));
    GRAPH_ASSERTD( (int) GRAPH_DIR_DOWN == (int) EDGE_LIST_SUCCS,
                   "Enums of direction and edge lists are not having right values");
    GRAPH_ASSERTD( (int) GRAPH_DIR_UP == (int) EDGE_LIST_PREDS,
                   "Enums of direction and edge lists are not having right values");
    edge->attach( dir, first_edge[ dir]); 
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

/** Default Constructor: creates 'end' iterator */
template < class EdgeIterImpl>
EdgeIterIface< EdgeIterImpl>::EdgeIterIface()
{

}

/** Constructor from node: iterator points on first edge, i.e. 'begin' iterator */
template < class EdgeIterImpl>
EdgeIterIface< EdgeIterImpl>::EdgeIterIface( Node *n):
    impl( n)
{

}

/** Copy constructor */
template < class EdgeIterImpl> 
EdgeIterIface< EdgeIterImpl>::EdgeIterIface( const EdgeIterIface& proto)
{
    impl = proto.impl;
}

/** Destructor */
template < class EdgeIterImpl> 
EdgeIterIface< EdgeIterImpl>::~EdgeIterIface()
{

}

/** Postincrement operator */
template < class EdgeIterImpl>
EdgeIterIface< EdgeIterImpl> 
EdgeIterIface< EdgeIterImpl>::operator++( int)
{
	EdgeIterIface tmp = *this;
    ++*this;
    return tmp;
}

/** Dereferenece operator*/
template < class EdgeIterImpl>
Edge * 
EdgeIterIface< EdgeIterImpl>::operator*()
{
    return impl.edge();
}
    
/** Comparison operator */
template < class EdgeIterImpl> 
bool
EdgeIterIface< EdgeIterImpl>::operator==(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return impl == o.impl;
} 

/** Not equals operator */
template < class EdgeIterImpl>
bool
EdgeIterIface< EdgeIterImpl>::operator!=(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return !(*this == o);
}

/** Get Edge */
template < class EdgeIterImpl>
Edge *
EdgeIterIface< EdgeIterImpl>::edge() const
{
    return impl.edge();
}

/** Get Edge */
template < class EdgeIterImpl>
Node *
EdgeIterIface< EdgeIterImpl>::node() const
{
    return impl.node();
}

/** Preincrement operator */
template < class EdgeIterImpl>
EdgeIterIface< EdgeIterImpl> & 
EdgeIterIface< EdgeIterImpl>::operator++()
{
    GRAPH_ASSERTD( isNotNullP( impl.edge()), "Edge iterator is at end ( NULL in edge pointer)");
    impl.nextEdge();
    return *this;
}

/** Next pred */
void PredIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "Edge iterator is at end ( NULL in edge_p pointer)");
    edge_p = edge_p->nextPred();
}


/** Next succ */
void SuccIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "Edge iterator is at end ( NULL in edge_p pointer)");
    edge_p = edge_p->nextSucc();
}

/** Preincrement operator */
void UnDirIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "Edge iterator is at end ( NULL in edge_p pointer)");
    
    if ( is_pred && isNullP( edge_p->nextPred()))
    {
        is_pred = false;
        edge_p = edge_p->succ()->firstSucc();
    } else
    {
        if ( is_pred)
        {
            edge_p = edge_p->nextPred();
        } else
        {
            edge_p = edge_p->nextSucc();
        }
    }
}

/** Get node on the end of edge */
Node *
SuccIterImpl::node() const
{
    return edge()->succ();
}

/** Get node on the end of edge */
Node *
PredIterImpl::node() const
{
    return edge()->pred();
}

/** Get node in UnDir traversal of node's edges */
Node*
UnDirIterImpl::node() const
{
    if ( is_pred)
    {
        return edge()->pred();
    } else
    {
        return edge()->succ();
    }    
}

/** Constructor gets first succ */
SuccIterImpl::SuccIterImpl( Node *n)
{
    edge_p = n->firstSucc();
}

/** Constructor gets first pred */
PredIterImpl::PredIterImpl( Node *n)
{
    edge_p = n->firstPred();
}


/** Constructor gets first edge for undirected edges iteration */
UnDirIterImpl::UnDirIterImpl( Node *n)
{
    edge_p = n->firstPred();
    is_pred = true;
    if ( isNullP( edge_p)) 
    {
        is_pred = false;
        edge_p = n->firstSucc();
    } 
}
#endif
