/**
 * @file: cfnode.h 
 * Description of CF node
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef CFNODE_H
#define CFNODE_H

/**
 * Representation of CF node
 */
class CFNode: public GNode
{
    Expr *first;
    Expr *last;
protected:    
    /** We can't create nodes separately, do it through newNode method of graph */
    CFNode( CFG *graph_p, int _id);
    /** Contructor of node with specified position */
    CFNode( CFG *graph_p, int _id, QPointF _pos);

    friend class CFG;
public:
    /** Destructor */
    virtual ~CFNode();
	
    CFG* graph() const;

    /** Get first expr */
    inline Expr *firstExpr() const
    {
        return first;
    }
    /** Get last expr */
    inline Expr *lastExpr() const 
    {
        return last;
    }
    /** Set first expr */
    inline void setFirstExpr( Expr *expr) 
    {
        first = expr;
    }
    /** Set last expr */
    inline void setLastExpr( Expr *expr)  
    {
        last = expr;
    }
    /** Get next graph's node */
    inline CFNode* nextNode()
    {
        return static_cast< CFNode*>( AuxNode::nextNode());
    }
    /** Get prev graph's node */
    inline CFNode* prevNode()
    {
        return static_cast< CFNode*>( AuxNode::prevNode());
    }
    /** Edge connection reimplementation */
    inline void AddEdgeInDir( CFEdge *edge, GraphDir dir)
    {
        AuxNode::AddEdgeInDir( (AuxEdge *)edge, dir);
    }
    /** Add predecessor */
    inline void AddPred( CFEdge *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }
    /** Add successor */
    inline void AddSucc( CFEdge *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    /** Get first edge in given direction */
    inline CFEdge* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< CFEdge*>( AuxNode::firstEdgeInDir( dir));
    }
    /** Get first successor */
    inline CFEdge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Get first predecessor */
    inline CFEdge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
};

#endif /* CFNODE_H */