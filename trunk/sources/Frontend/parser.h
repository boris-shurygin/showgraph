/**
 * @file: parser.h 
 * Base-level compiler dump parser
 */
/*
 * Frontend in ShowGraph tool.
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    /** LLVM bitcode dump */
    DUMP_TYPE_LLVM,
    /** Number of types */
    DUMP_TYPES_NUM
};

/** Type for position in dump file */
typedef qint64 DumpPos;

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
    /** Position of end in dump */
    DumpPos _end;
    /** Type */
    DumpType _type;
public:
    /** Constructor with id */
    DumpUnitInfo( int i);
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
    /** Get position in dump */
    inline DumpPos end() const
    {
        return _end;
    }
    /** Set position in dump */
    inline void setEnd( DumpPos p)
    {
        _end = p;
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
protected: 
    QFile file;
    QString curr_line;
    QList< DumpUnitInfo *> units;
    
    /** Type */
    DumpType _type;

    /** Statistics */
    DumpPos total_lines_num;
    DumpPos cur_line_num;

    /** Temporary internal data */
    DumpPos unit_beg;
    int unit_id;
    QString unit_name;
    QString phase_name;
    bool look_for_phase;    
    int num_blocks;
    bool unit_info_collected;
    DumpUnitInfo *cur_unit;

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
    /** End previous unit and save it */
    void unitEnd( DumpPos pos);
public:
    /** Constructor from given filename */
    Parser( QString filename);
    
    /** Destructor */
    virtual ~Parser();
    /** Convert input file in graph description XML */
    virtual void convert2XML( QString xmlname);
    virtual void preRun();
     
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

    /**
     * Collect info on dump units
     */
    virtual void collectInfo( DumpPos, QString line);

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
    inline DumpPos totalLinesNum() const
    {
        return total_lines_num;
    }
    inline DumpPos curLineNum() const
    {
        return cur_line_num;
    }
    /**
     * Get parser progress in percentage
     */
    int progress() const;
    /**
     * Find unit by given name and phase
     */    
    DumpUnitInfo *findUnit( QString name, QString phase);
    /**
     * Parse given unit
     */
    virtual void parseUnit( DumpUnitInfo *unit);
    /**
     * Parse unit with given name and phase
     */
    void parseUnit( QString name, QString phase);
    /**
     * Get list of routines in dump ( preRun must be executed first)
     */
    QStringList routines();
    /**
     * Get list of phases for given routine ( preRun must be executed first)
     */
    QStringList phases( QString name);
    /** Parse GCC dump */
    virtual void parseGCCUnit( DumpUnitInfo *unit);
    /** Parse Il0 dump */
    virtual void parseIL0Unit( DumpUnitInfo *unit);
    /** Parse LLVM dump */
    virtual void parseLLVMUnit( DumpUnitInfo *unit);
};

#endif