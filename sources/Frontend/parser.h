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
 * Types of IR dumps
 * @ingroup FE
 */
enum DumpType
{
    /** UNKNOWN type */
    DUMP_TYPE_UNKNOWN,
    /** GCC control flow dump */
    DUMP_TYPE_GCC_CF,
    /** ICC IL0 control flow dump */
    DUMP_TYPE_ICC_IL0,
    /** Number of types */
    DUMP_TYPES_NUM
};

/** Type for position in dump file */
typedef unsigned long int DumpPos;

/**
 * Describes a translation unit textual representation as part of dump
 * @ingroup FE
 */
class DumpUnitInfo
{
    /** Unit id */
    int _id;
    /** Name of translation unit */
    QString unit_name;
    /** Name of translation phase */
    QString phase_name;
    /** Position in dump */
    DumpPos _pos;
    /** Type */
    DumpType _type;
public:
    /** Constructor */
    DumpUnitInfo();
    /** Constructor with id */
    DumpUnitInfo( int i);
    /** Full constructor */
    DumpUnitInfo( int i, QString uname, QString phname, DumpPos p, DumpType type);
    /** Get id */
    inline int id() const
    {
        return _id;
    }
    /** Get unit name */
    inline QString unitName() const
    {
        return unit_name;
    }
    /** Set unit name */
    inline void setUnitName( QString name)
    {
        unit_name = name;
    }
    /** Get phase name */
    inline QString phaseName() const
    {
        return phase_name;
    }
    /** Set phase name */
    inline void setPhaseName( QString name)
    {
        phase_name = name;
    }
    /** Get position in dump */
    inline DumpPos pos() const
    {
        return _pos;
    }
    /** Set position in dump */
    inline void setPos( DumpPos p)
    {
        _pos = p;
    }
    /** Get type of dump */
    inline DumpType type() const
    {
        return _type;
    }
    /** Set type of dump */
    inline void setType( DumpType p)
    {
        _type = p;
    }
};

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
    QList< DumpUnitInfo *> units;

    /** Statistics */
    long long int total_lines_num;
    long long int cur_line_num;

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
    /** Parse one given line */
    virtual void parseLineDry( QString line)
    {
        return;
    }
public:
    /** Constructor from given filename */
    Parser( QString filename);
    
    /** Destructor */
    virtual ~Parser();
    /** Convert input file in graph description XML */
    virtual void convert2XML( QString xmlname);
    virtual void preRun();
    /**
     * Main loop routine that only builds parsing data structures 
     * Concurrent parsing should use this one
     */
    virtual void mainLoopDry();
    /** Main loop routine */
    virtual void mainLoop();
    /** Get currently processed line */
    inline QString curr() const
    {
        return curr_line;
    }
    inline long long int totalLinesNum() const
    {
        return total_lines_num;
    }
    inline long long int curLineNum() const
    {
        return cur_line_num;
    }
    /**
     * Get parser progress in percentage
     */
    int progress() const;
};

#endif