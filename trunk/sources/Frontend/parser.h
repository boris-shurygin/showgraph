/**
 * File: parser.h - General compiler dump parser
 * Component of Frontend library in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef PARSER_H
#define PARSER_H

/**
 * Provides generic functionality for parsing files. 
 * It opens/closes files, reads them line by line and calls specific parsing functions.
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

    ParserState _state;

protected:
    QString node_text;
    SymTab symtab;

    inline void setState( ParserState s)
    {
        _state = s;
    }
    inline ParserState state() const
    {
        return _state;
    }
    inline void setStateInit() { _state = PSTATE_INIT;};
    inline void setStateDefault() { _state = PSTATE_DEFAULT;};
    inline void setStateNode() { _state = PSTATE_NODE;};
    inline void setStateEdge() { _state = PSTATE_EDGE;};
    inline void setStateExpr() { _state = PSTATE_EXPR;};
    inline bool isStateDefault() const { return _state == PSTATE_DEFAULT;};
    inline bool isStateNode() const { return _state == PSTATE_NODE;};
    inline bool isStateEdge() const { return _state == PSTATE_EDGE;};
    inline bool isStateExpr() const { return _state == PSTATE_EXPR;};

    virtual bool nextLine( QString line)
    {
        return true;
    }
    virtual bool nodeStart( QString line)
    {
        return true;
    }
    virtual bool nodeStop( QString line)
    {
        return isStateNode() && nodeStart( line);
    }
    virtual void startNode()
    {
        return;
    }
    virtual void endNode()
    {
        return;
    }
    virtual void parseLine( QString line)
    {
        return;
    }
public:
    Parser( QString filename);
    ~Parser();
    
    virtual void convert2XML( QString xmlname);
    virtual void mainLoop();

    inline QString curr() const
    {
        return curr_line;
    }
};

#endif