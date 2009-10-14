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
class SymNode: public SymObj
{
    GNode *graph_node;
public:
    /** Constructor */
    SymNode( QString name): SymObj( name){};
    /** Set graph node */
    inline void setNode( GNode* n)
    {
        graph_node = n;
    }
    /** Get associated graph node */
    inline GNode* node() const
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
    GEdge* graph_edge;
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
    GNode *curr_node;
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
};

#endif