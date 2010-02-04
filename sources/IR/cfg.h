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
public:
    CFG();
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
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ)
    {
        return ( AuxEdge*)newEdge( static_cast< CFNode *>( pred),
                                    static_cast< CFNode *> (succ));
    }
    /** Reimplementation of newEdge virtual function of base class */
    AuxEdge* newEdge( AuxNode * pred, AuxNode *succ, QDomElement e)
    {
        return ( AuxEdge*)newEdge( static_cast< CFNode *>( pred),
                                    static_cast< CFNode *> (succ), e);
    }
    /** Node creation reimplementaiton */
    virtual void * CreateNode( AuxGraph *graph_p, int _id)
    {
        CFNode* node_p = new CFNode( static_cast<CFG *>(graph_p), _id);
        return node_p;
    }
    /** Edge creation reimplementation */
    virtual void * CreateEdge( AuxGraph *graph_p, int _id, AuxNode *_pred, AuxNode* _succ)
    {
        return new CFEdge(  static_cast<CFG *>( graph_p), _id,
                              static_cast<CFNode *>( _pred), 
                              static_cast<CFNode *>( _succ));
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
};

class CFView: public GraphView
{

};

#endif /* CFG_H */