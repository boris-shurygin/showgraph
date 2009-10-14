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

void 
Parser::mainLoop()
{
    if ( !file.open( QIODevice::ReadOnly))
    {
        return;
    }
    /** Read file line by line */
    QTextStream in( &file);
    QString line;

    /** Init state */
    setStateInit();

    do
    {
        curr_line = in.readLine();
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
}


void
Parser::convert2XML( QString xmlname)
{
    mainLoop();
}