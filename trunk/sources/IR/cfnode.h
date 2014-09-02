/**
 * @file: cfnode.h 
 * Description of CF node
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