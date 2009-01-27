/**
 * File: list.h - Some expiriments with list implementation. Not fully implemented. NOT FOR USE.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LIST_H
#define LIST_H

/**
 * Types of direction in lists.
 * Lists are built from left to right by default.
 * That means if you take next in default direction - it will be element to the right.
 */
enum ListDir
{
    LIST_DIR_RIGHT = 0,
    /** Defult direction */
    LIST_DIR_DEFAULT = LIST_DIR_RIGHT,
    LIST_DIR_LEFT = 1,
    LIST_DIR_RDEFAULT = LIST_DIR_LEFT,
    /** Number of directions */
    LIST_DIR_NUM = 2
};

/**
 * Base class for objects that should have pointers to next/prev objects of their type( i.e. behave like list elements)
 */
template <class Data> class ListBase
{
    Data * peer[ LIST_DIR_NUM];
public:
    /** Get neighbour */
    inline Data * GetPeerInDir( ListDir dir) const
    {
        return peer[ dir];
    }
    /** Set neighbour */
    inline void SetPeerInDir( ListDir dir, Data *data_p)
    {
        peer[ dir] = data_p;
    }
    /** Default peers gets */
    /** Return next peer in default direction */
    inline Data *GetNext() const
    {
        return GetPeerInDir( LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline Data* GetPrev() const
    {
        return GetPeerInDir( LIST_DIR_RDEFAULT);
    }
    /** Default peer sets */
    inline void SetNext( Data *n)
    {
        SetPeerInDir( LIST_DIR_DEFAULT, n);
    }
    inline void GetPrev( Data *p)
    {
        SetPeerInDir( LIST_DIR_RDEFAULT, p);
    }
};

#endif