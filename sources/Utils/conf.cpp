/**
 * @file: conf.cpp 
 * Implementation of configuration tools
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include "conf.h"

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