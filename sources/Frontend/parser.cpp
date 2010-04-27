/**
 * @file: parser.cpp
 * Implementation of generic IR parser
 */
/* 
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "fe_iface.h"

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
    
}

void Parser::preRun()
{
    total_lines_num = 0;
    if ( !file.open( QIODevice::ReadOnly))
    {
        return;
    }
    /** Read file line by line */
    QTextStream in( &file);
    QString line;

    do
    {
        line = in.readLine();
        total_lines_num++;
    } while ( !line.isNull());
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