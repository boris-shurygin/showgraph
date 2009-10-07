/**
 * @file: agraph.h 
 * Abstract Graph for testing of graph's properties and usage model.
 *
 * @defgroup AGr Test Graph
 *
 * @ingroup Graph
 * AGraph, ANode and AEdge classes present mimnimal code
 * that you need to write to employ Graph Library functionality.
 * AGraph classes differ from base in only one member of ( int) type
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef AGRAPH_H
#define AGRAPH_H

/* Predeclarations */
class ANode;
class AEdge;
class AGraph;

/**
 * Abstract node
 *
 * @ingroup AGr
 */
class ANode: public NodeT< AGraph, ANode, AEdge>
{
    int dummy;
    /** We can't create nodes separately, do it through newNode method of graph */
    ANode( AGraph *graph_p, int _id):
        NodeT< AGraph, ANode, AEdge>( graph_p, _id){};
    friend class GraphT< AGraph, ANode, AEdge>;
    friend class AGraph;
};

/**
 * Abstract edge
 *
 * @ingroup AGr
 */
class AEdge: public EdgeT< AGraph, ANode, AEdge>
{
    int dummy;

    /** Constructors are made private, only nodes and graph can create edges */
    AEdge( AGraph *graph_p, int _id, ANode *_pred, ANode* _succ):
        EdgeT< AGraph, ANode, AEdge>( graph_p, _id, _pred, _succ){};
        
    friend class GraphT< AGraph, ANode, AEdge>;
    friend class NodeT< AGraph, ANode, AEdge>;
    friend class AGraph;
    friend class ANode;
public:
    
};

/**
 * Testing-purpose graph
 *
 * @ingroup AGr
 */
class AGraph: public GraphT< AGraph, ANode, AEdge>
{
    int dummy; //Dummy class member

    public:

    /** Node creation overload */
    void * CreateNode( AGraph *graph_p, int _id)
    {
        return new ANode( graph_p, _id);
    }
	/** Edge creation overload */
    void * CreateEdge( AGraph *graph_p, int _id, ANode *_pred, ANode* _succ)
    {
        return new AEdge( graph_p, _id, _pred, _succ);
    }
};

#endif
