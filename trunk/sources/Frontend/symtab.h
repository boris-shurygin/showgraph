/**
 * @file: symtab.h 
 * Temporary symbol table for parser
 */
/*
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef SYMTAB_H
#define SYMTAB_H

/**
 * Types of symbolic objects
 * @ingroup FE
 */
enum SymType
{
    /** Any symbol*/
    SYM_GENERIC,

    /** Node */
    SYM_NODE,

    /** Edge */
    SYM_EDGE,

    /* Number of types */
    SYM_TYPES_NUM
};

/**
 * Base class for SymTab objects
 * @ingroup FE
 */
class SymObj
{
    /** Symbolic name */
    QString sym_name; 

public:
    /** Full Constructor */
    SymObj( QString name): sym_name( name){};

    /** Set object symbolic name */
    inline void setName( QString name)
    {
        sym_name = name;
    }
    /** Get object's symbolic name */
    inline QString name() const
    {
        return sym_name;
    }
    /** Get object's type */
    virtual SymType type() const
    {
        return SYM_GENERIC;
    }
};

/**
 * Symbol table
 * @ingroup FE
 */
class SymTab: public QHash<QString, SymObj *>
{
public:
    /** Empty destructor */
    ~SymTab()
    {

    }
};

#endif SYMTAB_H