/**
 * @file: expr.h 
 * Description of IR node ( expression)
 * @author Boris Shurygin
 */
/*
 * IR interface
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    CFNode *_node;
    /** Expression textual representation */
    QString string;
    /** Previous expression in chain */
    Expr *_prev;
    /** Next expression in chain */
    Expr *_next;
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
        return _node;
    }
    /** Set CF node */
    inline void setNode( CFNode * n)
    {
        _node = n;
    }
    /** Set string */
    inline void setString( QString str)
    {
        string = str;
    }
    /** Convert to string */
    inline QString toString() const
    {
        return string;
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
        return static_cast< Dep*>( AuxNode::firstEdgeInDir( dir));
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
        return _prev;
    }
    /** Get next expression */
    inline Expr *next() const
    {
        return _next;
    }
    /** Set previous expression */
    inline void setPrev( Expr * p)
    {
        _prev = p;
    }
    /** Set next expression */
    inline void setNext( Expr *n)
    {
        _next = n;
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