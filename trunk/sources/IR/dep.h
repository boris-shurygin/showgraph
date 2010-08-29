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
 * Types of dependence
 */
enum DepType
{
    /** Data flow */    
    DEP_TYPE_DF,
    
    /** Dependency of an any unknown nature */
    DEP_TYPE_ANY,

    /** Number of dependency types */
    DEP_TYPES_NUM
};

/**
 * Representation of dependence
 */
class Dep: public GEdge
{
    /** Type of dependecy */
    DepType dep_type;
protected:    
    /** Default constructor */
    Dep( IR *graph_p, int _id, Expr *_pred, Expr* _succ);


    friend class IR;
    friend class Expr;
public:
    /** Destructor */
    virtual ~Dep();

    /**
     * Return pointer to graph
     */
    IR *graph() const;

    /** Get type */
    inline DepType type() const 
    {
        return dep_type;
    }
    /** Set type */
    inline void setType( DepType t)
    {
        dep_type = t;
    }
    /** Check if dependency is of DF type */
    inline bool isDf() const
    {
        return dep_type == DEP_TYPE_DF;
    }
    /** Set dependency to be of DF type */
    inline void setDf()
    {
        dep_type = DEP_TYPE_DF;
    }
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