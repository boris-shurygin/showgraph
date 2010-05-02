/**
 * @file: fe_test.h 
 * Interface of testing-purpose frontend
 *
 * @defgroup FE_test Sample frontend
 * Sample frontend implementation use to
 * parse files with very simple graph representation
 * 
 * @ingroup FE
 */
/*
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef FE_TEST_H
#define FE_TEST_H

/**
 * Node representation as a symbol
 * @ingroup FE_test
 */
class SymExpr: public SymObj
{
    Expr *ir_expr;
public:
    /** Constructor */
    SymExpr( QString name): SymObj( name){};
    /** Set graph node */
    inline void setExpr( Expr* e)
    {
        ir_expr = e;
    }
    /** Get associated graph node */
    inline Expr* expr() const
    {
        return ir_expr;
    }
    /** Get node type */
    SymType type() const
    {
        return SYM_NODE;
    }
};

/**
 * Edge representation as a symbol
 * @ingroup FE_test
 */
class SymDep: public SymObj
{
    QString pred_name;
    QString succ_name;
    Dep* ir_dep;
public:
    /** Constructor */
    SymDep( QString name): SymObj( name){};
    /** Get symbol object type */
    SymType type() const
    {
        return SYM_EDGE;
    }
    /** Set the name of predecessor */
    inline void setPred( QString pred_string)
    {
        pred_name = pred_string;
    }
    /** Set the name of successor */
    inline void setSucc( QString succ_string)
    {
        succ_name = succ_string;
    }
    /** Get predecessor's name */
    inline QString pred() const
    {
        return pred_name;
    }
    /** Get successor's name */
    inline QString succ() const
    {
        return succ_name;
    }
};

/**
 * Node representation as a symbol
 * @ingroup FE_test
 */
class SymNode: public SymObj
{
    CFNode *graph_node;
public:
    /** Constructor */
    SymNode( QString name): SymObj( name){};
    /** Set graph node */
    inline void setNode( CFNode* n)
    {
        graph_node = n;
    }
    /** Get associated graph node */
    inline CFNode* node() const
    {
        return graph_node;
    }
    /** Get node type */
    SymType type() const
    {
        return SYM_NODE;
    }
};

/**
 * Edge representation as a symbol
 * @ingroup FE_test
 */
class SymEdge: public SymObj
{
    QString pred_name;
    QString succ_name;
    CFEdge* graph_edge;
public:
    /** Constructor */
    SymEdge( QString name): SymObj( name){};
    /** Get symbol object type */
    SymType type() const
    {
        return SYM_EDGE;
    }
    /** Set the name of predecessor */
    inline void setPred( QString pred_string)
    {
        pred_name = pred_string;
    }
    /** Set the name of successor */
    inline void setSucc( QString succ_string)
    {
        succ_name = succ_string;
    }
    /** Get predecessor's name */
    inline QString pred() const
    {
        return pred_name;
    }
    /** Get successor's name */
    inline QString succ() const
    {
        return succ_name;
    }
};

/**
 * Sample parser
 * @ingroup FE_test
 */
class TestParser: public Parser
{
    GraphView *graph;
    CFNode *curr_node;
public:
    /** Constructor */
    TestParser( QString str);
    /** Destructor */
    ~TestParser();
    /** Parse one given line procedure reimplementation */
    void parseLine( QString line);
    /** Check if this line starts a new node text section */
    bool nodeStart( QString line);
    /** Perform needed action to start processing a new node's text */
    void startNode();
    /** Finish the node text processing */
    void endNode();
    /** Convert the input file into XML graph description */
    void convert2XML( QString xmlname);
    /** Get the associated graph view */
    inline GraphView* graphView() const
    {
        return graph;
    }
    /** Check whether the given line should be treated as a next line */
    bool nextLine( QString line);
    /** Parse GCC dump */
    void parseGCCUnit( DumpUnitInfo *unit);
    /** Parse line in GCC dump */
    void parseLineGCC( QString line);
    /** Check if this line starts a new node text section */
    bool nodeStartGCC( QString line);
    /** Chech if the given line is the end of nde text section */
    inline  bool nodeStopGCC( QString line)
    {
        return isStateNode() && nodeStartGCC( line);
    }
    /** Check whether the given line should be treated as a next line */
    bool nextLineGCC( QString line);
    /** Parse Il0 dump */
    void parseIL0Unit( DumpUnitInfo *unit);
    /** Parse line in IL0 dump */
    void parseLineIL0( QString line);
    /** Check if this line starts a new node text section */
    bool nodeStartIL0( QString line);
    /** Check whether the given line should be treated as a next line */
    bool nextLineIL0( QString line);
    /** Chech if the given line is the end of nde text section */
    inline  bool nodeStopIL0( QString line)
    {
        return isStateNode() && nodeStartIL0( line);
    }
};

#endif