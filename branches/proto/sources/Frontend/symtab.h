/**
 * File: symtab.h - Temporary symbol table for parser
 * Component of Frontend library in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef SYMTAB_H
#define SYMTAB_H

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
 */
class SymObj
{
    /** Symbolic name */
    QString sym_name; 

public:
    /** Full Constructor */
    SymObj( QString name): sym_name( name){};

    inline void setName( QString name)
    {
        sym_name = name;
    }
    inline QString name() const
    {
        return sym_name;
    }
    
    virtual SymType type() const
    {
        return SYM_GENERIC;
    }
};

/** Temporary implementation */
class SymTab: public QHash<QString, SymObj *>
{
public:
    ~SymTab()
    {

    }
};

#endif SYMTAB_H