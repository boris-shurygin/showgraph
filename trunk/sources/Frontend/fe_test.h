/**
 * File: fe_gcc.h - Interface of test Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef FE_TEST_H
#define FE_TEST_H

class SymNode: public SymObj
{
    NodeItem *graph_node;
public:
    SymNode( QString name): SymObj( name){};
  
    inline void setNode( NodeItem* n)
    {
        graph_node = n;
    }
    inline NodeItem* node() const
    {
        return graph_node;
    }

    SymType type() const
    {
        return SYM_NODE;
    }
};

class SymEdge: public SymObj
{
    QString pred_name;
    QString succ_name;
    EdgeItem* graph_edge;
public:
    SymEdge( QString name): SymObj( name){};

    SymType type() const
    {
        return SYM_EDGE;
    }
    inline void setPred( QString pred_string)
    {
        pred_name = pred_string;
    }
    inline void setSucc( QString succ_string)
    {
        succ_name = succ_string;
    }
    inline QString pred() const
    {
        return pred_name;
    }
    inline QString succ() const
    {
        return succ_name;
    }
};


class TestParser: public Parser
{
    GraphView *graph;
public:
    TestParser( QString str);
    ~TestParser();

    void parseLine( QString line);

    void convert2XML( QString xmlname);
};

#endif