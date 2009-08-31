/**
 * File: parser.c - Implementation of generic IR parser
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "fe_iface.h"

/**
 * Contructor
 */
Parser::Parser( QString filename): file( filename)
{
    
}

/**
 * Parser destructor closes the file
 */
Parser::~Parser()
{
    
}

void
Parser::convert2XML(QString xmlname)
{
    QFile xmlfile( xmlname);
    
    if ( !file.open( QIODevice::ReadOnly))
    {
        return;
    }

    if ( !xmlfile.open( QFile::WriteOnly | QFile::Text))
    {
        return;
    }
    /** Read file line by line */
    QTextStream in( &file);
    QString line;
    do
    {
        curr_line = in.readLine();
        if ( !nextLine( curr_line))
        {
            line.append( curr_line);
        } else
        {
            if ( !line.isNull())
                parseLine( line);
            line = curr_line;
        }
    } while ( !curr_line.isNull());
}