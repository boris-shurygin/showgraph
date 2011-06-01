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

/** Parameter for iterator template specialization (pred traversal) */
struct PredIterImpl
{
    Edge *edge;    
};
/** Parameter for iterator template specialization (succ traversal) */
struct SuccIterImpl
{
    Edge *edge;    
};
/** Parameter for iterator template specialization (undirected traversal) */
struct UnDirIterImpl
{
    Edge *edge;
    bool is_pred;
};

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
    inline EdgeIterIface( Node *n);
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

/** Constructor specialization for preds */
template <>
EdgeIterIface< PredIterImpl>::EdgeIterIface( Node *n)
{
    impl.edge = n->firstPred();
}

/** Constructor specialization for succs */
template <>
EdgeIterIface< SuccIterImpl>::EdgeIterIface( Node *n)
{
    impl.edge = n->firstSucc();
}

/** Constructor specialization for undirected traversal */
template <>
EdgeIterIface< UnDirIterImpl>::EdgeIterIface( Node *n)
{
    impl.edge = n->firstPred();
    impl.is_pred = true;
    if ( isNullP( impl.edge))
    {
        impl.edge = n->firstSucc();
        impl.is_pred = false;
    }
}

/** Specialization for undirected traversal */
template <>
EdgeIterIface< UnDirIterImpl>::EdgeIterIface()
{
    impl.edge = NULL;
    impl.is_pred = false;
}

/** Constructor */
template < class EdgeIterImpl>
EdgeIterIface< EdgeIterImpl>::EdgeIterIface()
{
    impl.edge = NULL;
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

/** Preincrement operator */
template <>
EdgeIterIface< PredIterImpl> & 
EdgeIterIface< PredIterImpl>::operator++()
{
    GRAPH_ASSERTD( isNotNullP( impl.edge), "Edge iterator is at end ( NULL in edge pointer)");
    impl.edge = impl.edge->nextPred();
    return *this;
}

/** Preincrement operator */
template <>
EdgeIterIface< SuccIterImpl> & 
EdgeIterIface< SuccIterImpl>::operator++()
{
    GRAPH_ASSERTD( isNotNullP( impl.edge), "Edge iterator is at end ( NULL in edge pointer)");
    impl.edge = impl.edge->nextSucc();
    return *this;
}

/** Preincrement operator */
template <>
EdgeIterIface< UnDirIterImpl> & 
EdgeIterIface< UnDirIterImpl>::operator++()
{
    GRAPH_ASSERTD( isNotNullP( impl.edge), "Edge iterator is at end ( NULL in edge pointer)");
    
    if ( impl.is_pred && isNullP( impl.edge->nextPred()))
    {
        impl.is_pred = false;
        impl.edge = impl.edge->succ()->firstSucc();
    } else
    {
        if ( impl.is_pred)
        {
            impl.edge = impl.edge->nextPred();
        } else
        {
            impl.edge = impl.edge->nextSucc();
        }
    }
    return *this;
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
    return impl.edge;
}
    
/** Comparison operator */
template < class EdgeIterImpl> 
bool
EdgeIterIface< EdgeIterImpl>::operator==(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return impl.edge == o.impl.edge;
} 

/** Comparison operator specialization for undirected traversal */
template <> 
bool
EdgeIterIface< UnDirIterImpl>::operator==(const EdgeIterIface< UnDirIterImpl>& o) const
{
    return impl.edge == o.impl.edge
           && impl.is_pred == o.impl.is_pred;
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
    return impl.edge;
}

/** Get node on the end of edge */
template <>
Node *
EdgeIterIface< SuccIterImpl>::node() const
{
    return impl.edge->succ();
}

/** Get node on the end of edge */
template <>
Node *
EdgeIterIface< PredIterImpl>::node() const
{
    return impl.edge->pred();
}

/** Get node on the end of edge */
template <>
Node *
EdgeIterIface< UnDirIterImpl>::node() const
{
    if ( impl.is_pred)
    {
        return impl.edge->pred();
    } else
    {
        return impl.edge->succ();
    }
}

#endif
