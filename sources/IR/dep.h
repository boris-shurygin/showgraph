/**
 * @file: dep.h 
 * Description of IR edge ( dependency between expressions)
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef DEP_H
#define DEP_H

/**
 * Representation of dependence
 */
class Dep: public GEdge
{
protected:    
    /** Default constructor */
    Dep( IR *graph_p, int _id, Expr *_pred, Expr* _succ);
    /** Destructor */
    virtual ~Dep();

    friend class IR;
    friend class Expr;
public:
    /**
     * Return pointer to graph
     */
    IR *graph() const;

    /** Overload of setNode from base class */
    inline void setNode( Expr *n, GraphDir dir)
    {
        AuxEdge::setNode( ( AuxNode *)n, dir);
    }
    /** Set predecessor */
    inline void setPred( Expr *n)
    {
        setNode( n, GRAPH_DIR_UP);
    }
    /** Set successor */
    inline void setSucc( Expr *n)
    {
        setNode( n, GRAPH_DIR_DOWN);
    }
    /** Get node in given direction */
    inline Expr *node( GraphDir dir) const;
    /** Get predecessor */
    inline Expr *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /** Get successor */
    inline Expr *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }
    
    /** Get real node in given direction */
    inline Expr* realNode( GraphDir) const;

    /** Get real predecessor */
	inline Expr* realPred() const
	{
		return realNode( GRAPH_DIR_UP);
	}
	/** Get real successor */
	inline Expr* realSucc() const
	{
		return realNode( GRAPH_DIR_DOWN);
	}

    /** Next edge in graph's list */
    inline Dep* nextEdge()
    {
        return static_cast< Dep *>(AuxEdge::nextEdge());
    }
    /** Next edge in give direction */
    inline Dep* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< Dep *>(AuxEdge::nextEdgeInDir( dir));
    }
    /** Next successor */
    inline Dep* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Next predecessor */
    inline Dep* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
};

#endif /* Dep_H */