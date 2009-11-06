/**
 * @file: conf.h
 * Implementation of internal configuration of ShowGraph
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef CONF_H
#define CONF_H

#include <QString>
#include <QHash>

/**
 * Command line option type
 */
enum OptType
{
    /** Boolean */
    OPT_BOOL,
    /** Integer */
    OPT_INT,
    /** Float */
    OPT_FLOAT,
    /** String */
    OPT_STRING,
    /** Number of types */
    OPT_TYPES_NUM
};

/**
 * Command line option descrition
 */
class Option
{
    /** Type */
    OptType type;

    /** Option values */
    union OptValues
    {
        bool bool_val;
        int int_val;
        qreal float_val;
    } values;
    
    QString string_val;
    
    /** Short name */
    QString short_name;
    
    /** Long name */
    QString long_name;

    /** Description */
    QString descr;
public:
    /** Constructor without default val */
    Option( OptType _t, QString sname, QString lname, QString d):
        type( _t), short_name( sname), long_name( lname), descr( d){};

    /** Constructor with default bool val */
    Option( QString sname, QString lname, QString d, bool val):
        type( OPT_BOOL), short_name( sname), long_name( lname), descr( d)
    {
        values.bool_val = val;
    }

    /** Constructor for string option */
    Option( QString sname, QString lname, QString d):
        type( OPT_STRING), short_name( sname), long_name( lname), descr( d){};

    /** Get short name */
    inline QString shortName() const
    {
        return short_name;    
    }
    /** Get long name */
    inline QString longName() const
    {
        return long_name;    
    }
    /** Print option's synopsis and description */
    void print();
};

/**
 * Configuration class
 */
class Conf
{
    QHash< QString, Option *> short_opts;
    QHash< QString, Option *> long_opts;
public:
    /** Constructor */
    Conf();
    
    /** Destructor wipes all the options */
    ~Conf()
    {
        foreach( Option *opt, short_opts)
        {
            delete opt;
        }
    }
    /** Add option */
    inline void addOption( Option *opt)
    {
        short_opts[ opt->shortName()] = opt;
        long_opts[ opt->longName()] = opt;
    }

    /** Convenience routine: adds option without default val */
    inline void addOption( OptType _t, QString sname, QString lname, QString d)
    {
        Option *opt = new Option( _t, sname, lname, d);
        addOption( opt);
    }

    /** Convenience routine: adds option with default bool val */
    inline void addOption( QString sname, QString lname, QString d, bool val)
    {
        Option *opt = new Option( sname, lname, d, val);
        addOption( opt);
    }
    /** Convenience routine: adds string option without default value */
    inline void addOption( QString sname, QString lname, QString d)
    {
        Option *opt = new Option( sname, lname, d);
        addOption( opt);
    }
    
    /** Print options */
    void printOpts();

    /** Print value defaults */
    void printDefaults();
    /** Parse args */
    void readArgs( int argc, char** argv);
};

#endif