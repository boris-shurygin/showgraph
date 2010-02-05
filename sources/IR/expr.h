/**
 * @file: expr.h 
 * Description of IR node ( expression)
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef EXPR_H
#define EXPR_H

/**
 * Representation of expression
 */
class Expr: public GNode
{
    CFNode *cf_node;
protected:    
    /** We can't create nodes separately, do it through newNode method of graph */
    Expr( IR *graph_p, int _id);
    /** Contructor of node with specified position */
    Expr( IR *graph_p, int _id, QPointF _pos);

    friend class IR;
public:
    /** Destructor */
    virtual ~Expr();
	
    IR* graph() const;
    
    /** Get CF node */
    inline CFNode * node() const
    {
        return cf_node;
    }
    /** Set CF node */
    inline void setCFNode( CFNode * n)
    {
        cf_node = n;
    }
    /** Get next graph's node */
    inline Expr* nextNode()
    {
        return static_cast< Expr*>( AuxNode::nextNode());
    }
    /** Get prev graph's node */
    inline Expr* prevNode()
    {
        return static_cast< Expr*>( AuxNode::prevNode());
    }
    /** Edge connection reimplementation */
    inline void AddEdgeInDir( Dep *edge, GraphDir dir)
    {
        AuxNode::AddEdgeInDir( (AuxEdge *)edge, dir);
    }
    /** Add predecessor */
    inline void AddPred( Dep *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }
    /** Add successor */
    inline void AddSucc( Dep *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    /** Get first edge in given direction */
    inline Dep* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< Dep*>( AuxNode::first_edge[ dir]);
    }
    /** Get first successor */
    inline Dep* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Get first predecessor */
    inline Dep* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
};

#endif /* EXPR_H */