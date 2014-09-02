/**
 * @file: cfg.h 
 * Description of control flow graph
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
#ifndef CFG_H
#define CFG_H

/**
 * Control Flow Graph
 */
class CFG: public GGraph
{
    IR * priv_ir;
    TestParser *_parser;
protected:
    /** Node creation reimplementaiton */
    virtual Node * createNode( int _id)
    {
        CFNode* node_p = new ( node_pool) CFNode( this, _id);
        return node_p;
    }
    /** Edge creation reimplementation */
    virtual Edge * createEdge( int _id, Node *_pred, Node* _succ)
    {
        return new ( edge_pool) CFEdge(  this, _id,
                                         static_cast<CFNode *>( _pred), 
                                         static_cast<CFNode *>( _succ));
    }
public:
    /** Constructor */
    CFG( GraphView *v, bool create_pools);
    /** Destructor */
    ~CFG();
    /** New graphical node */
    CFNode* newNode();
    /** New graphical node */
    CFNode* newNode( QDomElement e);
    /** New graphical edge */
    CFEdge* newEdge( CFNode* pred, CFNode* succ);
    /** New graphical edge */
    CFEdge* newEdge( CFNode* pred, CFNode* succ, QDomElement e);
    
    /** Reimplementation of newEdge virtual function of base class */
    GEdge* newEdge( GNode * pred, GNode *succ)
    {
        return ( GEdge*)newEdge( static_cast< CFNode *>( pred),
                                    static_cast< CFNode *> (succ));
    }
    /** Reimplementation of newEdge virtual function of base class */
    GEdge* newEdge( GNode * pred, GNode *succ, QDomElement e)
    {
        return ( GEdge*)newEdge( static_cast< CFNode *>( pred),
                                    static_cast< CFNode *> (succ), e);
    }
    /** Get graph's first edge */
    inline CFEdge* firstEdge() 
    {
        return static_cast< CFEdge *>( AuxGraph::firstEdge());
    }
    /** Get graph's first node */
    inline CFNode* firstNode()
    {
        return static_cast< CFNode *>( AuxGraph::firstNode());
    }
    /** Get IR for CFG */
    inline IR *ir() const
    {
        return priv_ir;
    }
    /** Change IR of CFG */
    inline void setIR( IR *new_ir)
    {
        priv_ir = new_ir;
    }
    /** Get parser */
    inline TestParser * parser() const
    {
        return _parser;
    }
    /** Set parser */
    inline void setParser(TestParser * p)
    {
        _parser = p;
    }
};

/**
 * CFG view class
 */
class CFView: public GraphView
{

};

#endif /* CFG_H */