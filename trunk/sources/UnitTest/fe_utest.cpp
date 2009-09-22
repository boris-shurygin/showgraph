/**
 * File: fe_utest.cpp - Implementation of testing of Frontend, 
 * internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"

bool uTestFE()
{
    QString example_name( "example.txt");
    QFile example_file( example_name);
    
    if ( !example_file.open( QFile::WriteOnly | QFile::Text))
    {
        assert( 0);
        return false;
    }
    QTextStream outstream( &example_file);
    outstream << "Example file for parsing\n" 
              << "newline\n"
              << "  continue\n"
              << "newline\n"
              << "newline\n";
    example_file.close();
    
    /** Test generic parser */
    {
        class UTestParser: public Parser
        {
        public:
            UTestParser( QString str): Parser( str)
            {}

            bool nextLine( QString line)
            {
                QRegExp rx("cont");
                if ( rx.indexIn( line) != -1)
                    return false;
                return true;
            }
            void parseLine( QString line)
            {
                QTextStream stream( stdout);
                stream << line << endl;
            }
        };

        UTestParser parser( example_name);
        parser.convert2XML( QString( "xml.xml"));
    }
    return true;
}