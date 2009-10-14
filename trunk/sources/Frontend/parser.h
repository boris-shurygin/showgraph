/**
 * @file: parser.h 
 * Base-level compiler dump parser
 */
/*
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef PARSER_H
#define PARSER_H

/**
 * Provides generic functionality for parsing files. 
 * Generic parser just opens/closes files, reads them line by line 
 * and calls specific parsing functions.
 * @ingroup FE
 */
class Parser
{
    QFile file;
    QString curr_line;
        
    /** states of the parser */
    enum ParserState
    {
        /** initial state */
        PSTATE_INIT = 0x0,

        /** general text */
        PSTATE_DEFAULT = 0x1,
        
        /** inside node */
        PSTATE_NODE = 0x2,

        /** parsing edge */
        PSTATE_EDGE = 0x4,

        /** parsing expression */
        PSTATE_EXPR = 0x8
    };
    /** Parser's internal state */
    ParserState _state;

protected:
    /** Accumulation string for node text */
    QString node_text;
    /** Symbol table */
    SymTab symtab;
    /** Set parser state */
    inline void setState( ParserState s)
    {
        _state = s;
    }
    /** Get parser state */
    inline ParserState state() const
    {
        return _state;
    }
    /** Set state to initial */
    inline void setStateInit() { _state = PSTATE_INIT;};
    /** Set state to default */
    inline void setStateDefault() { _state = PSTATE_DEFAULT;};
    /** Set state to 'parsing node' */
    inline void setStateNode() { _state = PSTATE_NODE;};
    /** Set state to 'parsing edge' */
    inline void setStateEdge() { _state = PSTATE_EDGE;};
    /** Set state to 'parsing expression' */
    inline void setStateExpr() { _state = PSTATE_EXPR;};
    /** Check that state is default */
    inline bool isStateDefault() const { return _state == PSTATE_DEFAULT;};
    /** Check that state is 'parsing node' */
    inline bool isStateNode() const { return _state == PSTATE_NODE;};
    /** Check that state is 'parsing edge' */
    inline bool isStateEdge() const { return _state == PSTATE_EDGE;};
    /** Check that state is 'parsing expression' */
    inline bool isStateExpr() const { return _state == PSTATE_EXPR;};
    /** Check whether the given line should be treated as a next line */
    virtual bool nextLine( QString line)
    {
        return true;
    }
    /** Check if the given line starts new node text section */
    virtual bool nodeStart( QString line)
    {
        return true;
    }
    /** Chech if the given line is the end of nde text section */
    virtual bool nodeStop( QString line)
    {
        return isStateNode() && nodeStart( line);
    }
    /** Preform needed actions to start parsing node text */
    virtual void startNode()
    {
        return;
    }
    /** Finish node text parsing */
    virtual void endNode()
    {
        return;
    }
    /** Parse one given line */
    virtual void parseLine( QString line)
    {
        return;
    }
public:
    /** Constructor from given filename */
    Parser( QString filename);
    
    /** Destructor */
    ~Parser();
    /** Convert input file in graph description XML */
    virtual void convert2XML( QString xmlname);
    /** Main loop routine */
    virtual void mainLoop();
    /** Get currently processed line */
    inline QString curr() const
    {
        return curr_line;
    }
};

#endif