/**
 * @file: cfg.h 
 * Description of control flow graph
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
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