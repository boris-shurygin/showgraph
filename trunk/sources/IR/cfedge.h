/**
 * @file: cfedge.h 
 * Description of CF edge
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
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
    inline CFNode *node( GraphDir dir) const;
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
    inline CFNode* realNode( GraphDir) const;

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