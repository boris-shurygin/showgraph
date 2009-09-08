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
    GENERIC_SYMBOL,

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

    /** Object Type */
    SymType sym_type;

public:
    /** Full Constructor */
    SymObj(QString name, SymType tp): sym_name( name), sym_type( tp){};

    /** Access routines */
    inline void setType( SymType tp)
    {
        sym_type = tp;
    }
    inline void setName( QString name)
    {
        sym_name = name;
    }
    inline QString name() const
    {
        return sym_name;
    }
    inline SymType type() const
    {
        return sym_type;
    }
};

typedef QHash<QString, SymObj *> SymTab;

#endif SYMTAB_H