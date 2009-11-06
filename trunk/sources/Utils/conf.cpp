/**
 * @file: conf.cpp 
 * Implementation of configuration tools
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include "conf.h"


#include <QRegExp>

/** Option's print routine */
void
Option::print()
{
    QTextStream stream( stdout);
    
    stream << "-" << short_name << ", "
           << "--" << long_name << "    "
           << descr << endl;
}

/** Default constructor of configuration */
Conf::Conf()
{
#ifdef _DEBUG
    /* Debug options */

#endif
    /* General purpose options */
}

/** Print options */
void Conf::printOpts()
{
    foreach( Option *opt, short_opts)
    {
        opt->print();
    }
}

/** Print value defaults */
void Conf::printDefaults()
{

}

/** Read input args */
void Conf::readArgs( int argc, char** argv)
{
    QTextStream err( stderr);
    
    for ( int i = 0; i < argc; i++)
    {
        QString curr( argv[ i]);
        QRegExp short_rx("^-([^-]+)");
        QRegExp long_rx("^--([^-]+)");
        if ( short_rx.indexIn( curr) != -1 )
        {
            /* We look for expression among short option names */
            QString name = short_rx.cap( 1);
            
            if ( short_opts.find( name) != short_opts.end())
            {
                Option *opt =  short_opts[ name];
            } else
            {
                err << "No such option " << name << endl;
            }
        } else if (  long_rx.indexIn( curr) != -1)
        {
            /* We look for expression among long option names */
            QString name = long_rx.cap( 1);
            if ( long_opts.find( name) != long_opts.end())
            {
                Option *opt =  long_opts[ name];
            } else
            {
                err << "No such option " << name << endl;
            }
        } else
        {
            /** Is not an option specifier */
            err << "Unrecognized argument " << curr << endl;
        }
    }
}