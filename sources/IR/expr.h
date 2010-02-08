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
    /** Expression's CF node */
    CFNode *cf_node;
    /** Textual representation */
    QString string;
    /** Previous expression in chain */
    Expr *prev_expr;
    /** Next expression in chain */
    Expr *next_expr;
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
    /** Get previous expression */
    inline Expr *prev() const
    {
        return prev_expr;
    }
    /** Get next expression */
    inline Expr *next() const
    {
        return next_expr;
    }
    /** Set previous expression */
    inline void setPrev( Expr * p)
    {
        prev_expr = p;
    }
    /** Set next expression */
    inline void setNext( Expr *n)
    {
        next_expr = n;
    }
    /** Insert expression in sequence */
    inline void insert( Expr *prev)
    {
        assert( isNotNullP( prev));
        Expr *n = prev->next();
        
        /** Connect to prev */
        setPrev( prev);
        prev->setNext( this);
        
        /** Connect to next */
        if ( isNotNullP( n))
        {
            n->setPrev( this);
            setPrev( n);
        }
    }
    /** Detach from prev and next */
    inline void detach()
    {
        if ( isNotNullP( prev()))
        {
            prev()->setNext( next());
        }
        if ( isNotNullP( next()))
        {
            next()->setPrev( prev());
        }
    }
};

#endif /* EXPR_H */