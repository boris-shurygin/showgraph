/**
 * @file: list.h 
 * Some expiriments with list implementation
 * @defgroup List Two-way linked list
 *
 * Implementation of two-way linked list
 * @ingroup Utils
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LIST_H
#define LIST_H

/**
 * Types of direction in lists.
 * Lists are built from left to right by default.
 * That means if you take next in default direction - it will be element to the right.
 * @ingroup List
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
 * Return direction that is reverse to given one
 * @ingroup List
 */
inline ListDir
ListRDir( ListDir dir)
{
    assert( LIST_DIR_NUM == 2);
    return ( dir == LIST_DIR_DEFAULT)? LIST_DIR_RDEFAULT: LIST_DIR_DEFAULT; 
}


/**
 * Class for objects that should have pointers to next/prev objects of their type( behave like list elements)
 * Implements headless list data structure
 * @ingroup List
 */
template <class Data> class ListItem
{
    ListItem<Data> * peer[ LIST_DIR_NUM];
    Data *data_p;
public:
    
    /**Get data */
    inline Data *data() const
    {
        return data_p;
    }
    /** Set data */
    inline void setData( Data* d)
    {
        data_p = d;
    }
    
    /** Get neighbour */
    inline ListItem<Data> * GetPeerInDir( ListDir dir) const
    {
        return peer[ dir];
    }
    
    /** Set neighbour */
    inline void SetPeerInDir( ListItem<Data> *p, ListDir dir)
    {
        peer[ dir] = p;
    }
    /** Default peers gets */
    /** Return next peer in default direction */
    inline ListItem<Data> *next() const
    {
        return GetPeerInDir( LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline ListItem<Data>* GetPrev() const
    {
        return GetPeerInDir( LIST_DIR_RDEFAULT);
    }
    /** Set next peer */
    inline void SetNext( ListItem<Data> *n)
    {
        SetPeerInDir( n, LIST_DIR_DEFAULT);
    }
    /** Set previous peer */
    inline void SetPrev( ListItem<Data> *p)
    {
        SetPeerInDir( p, LIST_DIR_RDEFAULT);
    }
    
    /** Attach this item to peeer in give direction */
    inline void AttachInDir( ListItem<Data>* p, ListDir dir)
    {
        ListDir rdir = ListRDir( dir);
        SetPeerInDir( p, dir);
        SetPeerInDir( NULL, rdir);

        if ( isNotNullP( p))
        {
            ListItem<Data>* rdir_peer = p->GetPeerInDir( rdir);
            if ( isNotNullP( rdir_peer))
            {
                rdir_peer->SetPeerInDir( this, dir);
            }
            p->SetPeerInDir( this, rdir);
            SetPeerInDir( rdir_peer, rdir);
        }
    }
    
    /** Attach in default direction */
    inline void Attach( ListItem<Data>* peer)
    {
        AttachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Detach from neighbours */
    inline void Detach()
    {
        /** Correct links in peers */
        if ( isNotNullP( peer[ LIST_DIR_DEFAULT]))
        {
            peer[ LIST_DIR_DEFAULT]->SetPeerInDir( peer[ LIST_DIR_RDEFAULT], LIST_DIR_RDEFAULT);
        }
        if ( isNotNullP( peer[ LIST_DIR_RDEFAULT]))
        {
            peer[ LIST_DIR_RDEFAULT]->SetPeerInDir( peer[ LIST_DIR_DEFAULT], LIST_DIR_DEFAULT);
        }
        SetPeerInDir( NULL, LIST_DIR_DEFAULT);
        SetPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }

    /** Default constructor */
    ListItem()
    {
        setData( NULL);
        SetPeerInDir( NULL, LIST_DIR_DEFAULT);
        SetPeerInDir( NULL, LIST_DIR_RDEFAULT);
    };

    /** Constructor from data pointer */
    ListItem( Data* d)
    {
        setData( d);
        SetPeerInDir( NULL, LIST_DIR_DEFAULT);
        SetPeerInDir( NULL, LIST_DIR_RDEFAULT);
    };

    /** Insert element before the given one */
    ListItem( ListItem<Data> *peer, Data* d)
    {
        setData( d);
        SetPeerInDir( NULL, LIST_DIR_DEFAULT);
        SetPeerInDir( NULL, LIST_DIR_RDEFAULT);
        AttachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Insert element in given direction */
    ListItem( ListItem<Data> *peer, ListDir dir, Data *d)
    {
        setData( d);
        SetPeerInDir( NULL, LIST_DIR_DEFAULT);
        SetPeerInDir( NULL, LIST_DIR_RDEFAULT);
        AttachInDir( peer, dir);
    }

    /** Destructor */
    ~ListItem()
    {
        Detach();
    }
};

#endif