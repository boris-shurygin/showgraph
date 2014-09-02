/**
 * @file: cfedge.h 
 * Description of CF edge
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
#ifndef CFEDGE_H
#define CFEDGE_H

/**
 * Representation of control flow edge
 */
class CFEdge: public GEdge
{
protected:    
    /** Default constructor */
    CFEdge( CFG *graph_p, int _id, CFNode *_pred, CFNode* _succ);

    friend class CFG;
    friend class CFNode;
public:
    /** Destructor */
    virtual ~CFEdge();
    
    /**
     * Return pointer to graph
     */
    CFG *graph() const;

    /** Overload of setNode from base class */
    inline void setNode( CFNode *n, GraphDir dir)
    {
        AuxEdge::setNode( ( AuxNode *)n, dir);
    }
    /** Set predecessor */
    inline void setPred( CFNode *n)
    {
        setNode( n, GRAPH_DIR_UP);
    }
    /** Set successor */
    inline void setSucc( CFNode *n)
    {
        setNode( n, GRAPH_DIR_DOWN);
    }
    /** Get node in given direction */
    CFNode *node( GraphDir dir) const;
    /** Get predecessor */
    inline CFNode *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /** Get successor */
    inline CFNode *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }
    
    /** Get real node in given direction */
    CFNode* realNode( GraphDir) const;

    /** Get real predecessor */
    inline CFNode* realPred() const
    {
        return realNode( GRAPH_DIR_UP);
    }
    /** Get real successor */
    inline CFNode* realSucc() const
    {
        return realNode( GRAPH_DIR_DOWN);
    }

    /** Next edge in graph's list */
    inline CFEdge* nextEdge()
    {
        return static_cast< CFEdge *>(AuxEdge::nextEdge());
    }
    /** Next edge in give direction */
    inline CFEdge* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< CFEdge *>(AuxEdge::nextEdgeInDir( dir));
    }
    /** Next successor */
    inline CFEdge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Next predecessor */
    inline CFEdge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
};

#endif /* CFEDGE_H */