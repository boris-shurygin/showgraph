/**
 * @file: symtab.h 
 * Temporary symbol table for parser
 */
/*
 * Frontend in ShowGraph tool.
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    /** Destructor */
    virtual ~SymObj(){};

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
        removeObjects();
    }
    /** Remove all objects */
    void removeObjects()
    {
         QHash<QString, SymObj *>::const_iterator i = constBegin();
         while (i != constEnd())
         {
             delete i.value();
             ++i;
         } 
         clear();
    }
};

#endif // SYMTAB_H