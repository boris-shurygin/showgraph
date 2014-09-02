/**
 * @file: parser.cpp
 * Implementation of generic IR parser
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
#include "fe_iface.h"

#ifdef _DEBUG
#  define SHOW_UNITS
#endif

/**
 * Convert dump type to the descripting string
 */
inline QString
typeToStr( DumpType tp)
{
    switch ( tp)
    {
    case DUMP_TYPE_UNKNOWN:
        return QString("unknown");
    case DUMP_TYPE_GCC_CF:
        return QString("GCC CF dump");
    case DUMP_TYPE_ICC_IL0:
        return QString("IL0 CF dump");
    case DUMP_TYPE_LLVM:
        return QString("LLVM Bitcode text");
    default:
        return QString("");
    }
    return QString("");
}

/** Constructor with id */
DumpUnitInfo::DumpUnitInfo( int i): _id( i)
{
    _pos = 0;
    _end = 0;
    _type = DUMP_TYPE_UNKNOWN;
}

/**
 * Constructor
 */
Parser::Parser( QString filename): file( filename)
{
    setStateInit();
}

/**
 * Parser destructor closes the file
 */
Parser::~Parser()
{
    foreach( DumpUnitInfo *unit, units)
    {
        delete unit;
    }
}

/**
 * Find unit by given name and phase
 */    
DumpUnitInfo *
Parser::findUnit( QString name, QString phase)
{
    foreach( DumpUnitInfo *unit, units)
    {
        if ( unit->unitName() == name
             && unit->phaseName() == phase)
            return unit;
    }
    return NULL;
}

/**
 * Parse given unit
 */
void 
Parser::parseUnit( DumpUnitInfo *unit)
{
    setType( unit->type());
    switch( unit->type())
    {
    case DUMP_TYPE_UNKNOWN:
        return;
    case DUMP_TYPE_GCC_CF:
        parseGCCUnit( unit);
        return;
    case DUMP_TYPE_ICC_IL0:
        parseIL0Unit( unit);
        return;
    case DUMP_TYPE_LLVM:
        parseLLVMUnit( unit);
        return;
    default:
        return;
    }
    return;
}

/** Parse GCC dump */
void Parser::parseGCCUnit( DumpUnitInfo *unit)
{

}
/** Parse Il0 dump */
void Parser::parseIL0Unit( DumpUnitInfo *unit)
{

}
/** Parse LLVM dump */
void Parser::parseLLVMUnit( DumpUnitInfo *unit)
{

}

/**
 * Parse unit with given name and phase
 */
void
Parser::parseUnit( QString name, QString phase)
{
    DumpUnitInfo *unit = findUnit( name, phase);
    if ( isNotNullP( unit))
    {   
        parseUnit( unit);
    }
}
/**
 * Get list of routines in dump ( preRun must be executed first)
 */
QStringList
Parser::routines()
{
    QStringList list;
    QHash< QString, DumpUnitInfo *> hash;
    foreach( DumpUnitInfo *unit, units)
    {
        hash[ unit->unitName()] = unit;
    }
    foreach( QString name, hash.keys())
    {
        list.push_back( name);
    }
    return list;
}
/**
 * Get list of phases for given routine ( preRun must be executed first)
 */
QStringList 
Parser::phases( QString name)
{
    QStringList list;
    foreach( DumpUnitInfo *unit, units)
    {
        if ( unit->unitName() == name)
            list.push_back( unit->phaseName());
    }
    return list;
}

void
Parser::unitEnd( DumpPos pos)
{
    if ( type() == DUMP_TYPE_UNKNOWN)
        return;
#ifdef SHOW_UNITS
    QTextStream stream( stdout);
    stream << "Unit " << unit_id << ":" << endl;
    stream << " Unit name: " << ( unit_name.isEmpty() ? "unknown" : unit_name) << endl;
    stream << " Phase name: " << phase_name << endl;
    stream << " Type: " << typeToStr( type()) << endl;
#endif    
    DumpUnitInfo *unit = new DumpUnitInfo( unit_id++);
    units.push_back( unit);
    unit->setPos( unit_beg);
    unit->setEnd( pos);
    unit->setType( type());
    unit->setPhaseName( phase_name);
    unit->setUnitName( unit_name);
    setType( DUMP_TYPE_UNKNOWN);
    unit_info_collected = false;
}

void 
Parser::collectInfo( DumpPos pos, QString line)
{
    /** Determine type of dump */
    if ( line.indexOf("After") != -1 || line.indexOf("Before") != -1)
    {
        unitEnd( pos); // End previous unit 
        /** Remember that it might be a phase beginning in icc IL0 dump */
        QRegExp phase_rx("([^:]+):");
        if ( phase_rx.indexIn( line) != -1 )
        {
            phase_name = phase_rx.cap( 1);
            unit_beg = pos;
            _type = DUMP_TYPE_ICC_IL0;
            unit_info_collected = false;
        }
    }
    if ( line.indexOf(";; Function") != -1)
    {
        unitEnd( pos);
        /** Remember that it might be a phase beginning in icc IL0 dump */
        QRegExp fname_gcc_rx(";; Function (\\S+)");
        if ( fname_gcc_rx.indexIn( line) != -1 )
        {
            phase_name = "unknown";
            unit_name = fname_gcc_rx.cap(1);
            unit_beg = pos;
            _type = DUMP_TYPE_GCC_CF;
            look_for_phase = true;
            num_blocks = -1;
            unit_info_collected = false;
        }        
    }

    if ( line.indexOf("define") != -1)
    {
        /** Remember that it might be a phase beginning in icc IL0 dump */
        QRegExp fname_llvm_rx("^define (\\S+) @(\\S+)\\(");
        if ( fname_llvm_rx.indexIn( line) != -1 )
        {
            unitEnd( pos);
            phase_name = "unknown";
            unit_name = fname_llvm_rx.cap(2);
            unit_beg = pos;
            _type = DUMP_TYPE_LLVM;
            num_blocks = -1;
            unit_info_collected = false;
        }        
    }
    
    if ( type() == DUMP_TYPE_UNKNOWN 
         || unit_info_collected)
        return;

    /** If we have already discovered the type */
    if ( _type == DUMP_TYPE_ICC_IL0 && line.indexOf("entry") != -1)
    {
        int i = 0;
        QString name_str = line;
        if ( ( i = name_str.indexOf("(")) != -1)
        {
            name_str.truncate( i);
        }
        QRegExp fname_icc_rx("(\\S+)$");
        if ( fname_icc_rx.indexIn( name_str) != -1)
        {
            unit_name = fname_icc_rx.cap( 1);
            unit_info_collected = true;
        }
    } else if ( _type == DUMP_TYPE_GCC_CF)
    {
        if ( look_for_phase && num_blocks == -1)
        {
            QRegExp numblock_rx("(\\d+) basic blocks");
            if ( numblock_rx.indexIn( line) != -1)
            {
                num_blocks = numblock_rx.cap(1).toInt();
            }

        } else if ( num_blocks > 0)
        {
            QRegExp block_rx("Basic block (\\d+)");
            if ( block_rx.indexIn( line) != -1)
            {
                num_blocks--;
            }
        } else if ( look_for_phase && num_blocks == 0)
        {
            QRegExp phase_name_rx("^(\\S+)");
            if ( phase_name_rx.indexIn( line) == -1)
            {
                num_blocks = -2;
            }
        } else if ( look_for_phase && num_blocks == -2)
        {
            QRegExp phase_name_rx("^(\\S+.*)");
            if ( phase_name_rx.indexIn( line) != -1)
            {
                phase_name = phase_name_rx.cap(1);
                look_for_phase = false;
                unit_info_collected = true;
                num_blocks = -1;
            }
        }
    } else if ( _type == DUMP_TYPE_LLVM && line.indexOf("entry") != -1)
    {
        unit_info_collected = true;
    }
}

void
Parser::preRun()
{
    total_lines_num = 0;
    if ( !file.open( QIODevice::ReadOnly))
    {
        return;
    }
    /** Read file line by line */
    QTextStream in( &file);
    QString line;
    unit_beg = 0;
    unit_id = 0;
    setType( DUMP_TYPE_UNKNOWN);
    do
    {
        line = in.readLine();
        collectInfo( total_lines_num, line);
        total_lines_num++;
    } while ( !line.isNull());
    if ( type() != DUMP_TYPE_UNKNOWN)
    {
        unitEnd( total_lines_num);
    }
}

int Parser::progress() const
{
    return 100 * ( cur_line_num / total_lines_num);
}
void 
Parser::mainLoopDry()
{
    cur_line_num = 0;
    if ( !file.open( QIODevice::ReadOnly))
    {
        return;
    }
    /** Read file line by line */
    QTextStream in( &file);
    QString line;

    /** Init state */
    setStateInit();
#ifdef _DEBUG
        out( "Started parsing");
#endif
    do
    {
        curr_line = in.readLine();
        cur_line_num++;
        if ( !nextLine( curr_line))
        {
            line.append( curr_line);
        } else
        {
            if ( !line.isNull())
            {
                if ( nodeStop( line))
                {
                    endNode();
                    setStateDefault();
                }
                if ( nodeStart( line))
                {
                    setStateNode();
                    startNode();
                }             
                parseLineDry( line);
            }
            line = curr_line;
        }
    } while ( !curr_line.isNull());
    if ( isStateNode())
    {
        endNode();
    }
#ifdef _DEBUG
        out( "Finished parsing");
#endif
}

void 
Parser::mainLoop()
{
    cur_line_num = 0;
    if ( !file.isOpen() && !file.open( QIODevice::ReadOnly))
    {
        return;
    }
    file.reset();

    /** Read file line by line */
    QTextStream in( &file);
    QString line;

    /** Init state */
    setStateInit();
#ifdef _DEBUG
        out( "Started parsing");
#endif
    do
    {
        curr_line = in.readLine();
        cur_line_num++;
        if ( !nextLine( curr_line))
        {
            line.append( curr_line);
        } else
        {
            if ( !line.isNull())
            {
                if ( nodeStop( line))
                {
                    endNode();
                    setStateDefault();
                }
                if ( nodeStart( line))
                {
                    setStateNode();
                    startNode();
                }             
                parseLine( line);
            }
            line = curr_line;
        }
    } while ( !curr_line.isNull());
    if ( isStateNode())
    {
        endNode();
    }
#ifdef _DEBUG
        out( "Finished parsing");
#endif
}


void
Parser::convert2XML( QString xmlname)
{
    mainLoop();
}