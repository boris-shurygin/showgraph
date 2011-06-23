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
    /** Right direction */
    LIST_DIR_RIGHT = 0,
    /** Default direction */
    LIST_DIR_DEFAULT = LIST_DIR_RIGHT,
    /** Left Direction */
    LIST_DIR_LEFT = 1,
    /** Direction reverse to default */
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
 * Container for list elements. Data is accessed via pointer
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
    inline ListItem<Data>* prev() const
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

/** List number */
typedef quint16 ListId;

/**
 * Class for objects that should have pointers to next/prev objects of their type( behave like list elements)
 * Inherited object can be member of several lists
 * Implements headless list data structure
 * Data is accessed by static cast -> less overhead since no additional indirection
 * @ingroup List
 */
template <unsigned int dim> class MListItem
{
    MListItem< dim> * peer[ dim][ LIST_DIR_NUM];
public:
    
        /** Get neighbour */
    inline MListItem< dim> * peerInDir( ListId list, ListDir dir) const
    {
        ASSERTD( list < dim);
        return peer[ list][ dir];
    }
    
    /** Set neighbour */
    inline void setPeerInDir( ListId list, MListItem< dim> *p, ListDir dir)
    {
        ASSERTD( list < dim);
        peer[list][ dir] = p;
    }
    /** Set all pointeers to peeers to zero */
    inline void zeroLinks()
    {
        for ( ListId list = 0; list < dim; list++)
        {
            setPeerInDir( list, NULL, LIST_DIR_DEFAULT);
            setPeerInDir( list, NULL, LIST_DIR_RDEFAULT);
        }
    }
    /** Default peers gets */
    /** Return next peer in default direction */
    inline MListItem< dim> *next( ListId list) const
    {
        return peerInDir( list, LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline MListItem< dim>* prev( ListId list) const
    {
        return peerInDir( list, LIST_DIR_RDEFAULT);
    }
    /** Set next peer */
    inline void setNext( ListId list, MListItem< dim> *n)
    {
        setPeerInDir( list, n, LIST_DIR_DEFAULT);
    }
    /** Set previous peer */
    inline void setPrev( ListId list, MListItem< dim> *p)
    {
        setPeerInDir( list, p, LIST_DIR_RDEFAULT);
    }
    
    /** Attach this item to peeer in give direction */
    inline void attachInDir( ListId list, MListItem< dim>* p, ListDir dir)
    {
        ListDir rdir = ListRDir( dir);
        setPeerInDir( list, p, dir);
        setPeerInDir( list, NULL, rdir);

        if ( isNotNullP( p))
        {
            MListItem< dim>* rdir_peer = p->peerInDir( list, rdir);
            if ( isNotNullP( rdir_peer))
            {
                rdir_peer->setPeerInDir( list, this, dir);
            }
            p->setPeerInDir( list, this, rdir);
            setPeerInDir( list, rdir_peer, rdir);
        }
    }
    
    /** Attach in default direction */
    inline void attach( ListId list, MListItem< dim>* peer)
    {
        attachInDir( list, peer, LIST_DIR_DEFAULT);
    }

    /** Detach from neighbours */
    inline void detach( ListId list)
    {
        ASSERTD( list < dim);
        /** Correct links in peers */
        if ( isNotNullP( peer[ list][ LIST_DIR_DEFAULT]))
        {
            peer[ list][ LIST_DIR_DEFAULT]->setPeerInDir( list, peer[ list][ LIST_DIR_RDEFAULT], LIST_DIR_RDEFAULT);
        }
        if ( isNotNullP( peer[ list][ LIST_DIR_RDEFAULT]))
        {
            peer[ list][ LIST_DIR_RDEFAULT]->setPeerInDir( list, peer[ list][ LIST_DIR_DEFAULT], LIST_DIR_DEFAULT);
        }
        setPeerInDir( list, NULL, LIST_DIR_DEFAULT);
        setPeerInDir( list, NULL, LIST_DIR_RDEFAULT);
    }
    /** Detach from all lists */
    inline void detachAll()
    {
        for ( ListId list = 0; list < dim; list++)
        {
            detach( list);
        }    
    }

    /** Default constructor */
    MListItem()
    {
        zeroLinks();
    };

    /** Insert element before the given one */
    MListItem( ListId list, MListItem< dim> *peer)
    {
        zeroLinks();
        attachInDir( list, peer, LIST_DIR_DEFAULT);
    }

    /** Insert element in given direction */
    MListItem( ListId list, MListItem< dim> *peer, ListDir dir)
    {
        zeroLinks();
        attachInDir( list, peer, dir);
    }

    /** Destructor */
    virtual ~MListItem()
    {
        detachAll();
    }
};

/**
 * Interface for Multi-list
 * @ingroup List
 */
template < class Item, class ListBase, unsigned int dim> class MListIface: public ListBase
{
public:
    /** Return next item in default direction */
    inline Item *next( ListId list) const
    {
        return static_cast< Item *>( MListItem< dim>::next( list));
    }
    /** Return prev item in default direction */
    inline Item* prev( ListId list) const
    {
        return static_cast< Item *>( MListItem< dim>::prev( list));
    }
    /** Default constructor */
    inline MListIface():
        ListBase(){};

    /** Insert element before the given one */
    inline MListIface( ListId list, Item *peer):
        ListBase( list, peer){};

    /** Insert element in given direction */
    inline MListIface( ListId list, Item *peer, ListDir dir):
        ListBase( list, peer, dir){};
};

/**
 * Class for objects that should have pointers to next/prev objects of their type( behave like list elements)
 * Specilized for one list
 * Implements headless list data structure
 * Data is accessed by static cast -> less overhead since no additional indirection
 * @ingroup List
 */
template<> class MListItem<1>
{
    MListItem< 1> * peer[ LIST_DIR_NUM];
    
    /** Get neighbour */
    inline MListItem< 1> * peerInDir( ListDir dir) const
    {
        return peer[ dir];
    }
    
    /** Set neighbour */
    inline void setPeerInDir( MListItem< 1> *p, ListDir dir)
    {
        peer[ dir] = p;
    }

public:
    /** Set all pointeers to peeers to zero */
    inline void zeroLinks()
    {
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }    
    /** Default peers gets */
    /** Return next peer in default direction */
    inline MListItem< 1> *next() const
    {
        return peerInDir( LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline MListItem< 1>* prev() const
    {
        return peerInDir( LIST_DIR_RDEFAULT);
    }
    /** Set next peer */
    inline void setNext( MListItem< 1> *n)
    {
        setPeerInDir( n, LIST_DIR_DEFAULT);
    }
    /** Set previous peer */
    inline void setPrev( MListItem< 1> *p)
    {
        setPeerInDir( p, LIST_DIR_RDEFAULT);
    }
    
    /** Attach this item to peeer in give direction */
    inline void attachInDir( MListItem< 1>* p, ListDir dir)
    {
        ListDir rdir = ListRDir( dir);
        setPeerInDir( p, dir);
        setPeerInDir( NULL, rdir);

        if ( isNotNullP( p))
        {
            MListItem< 1>* rdir_peer = p->peerInDir( rdir);
            if ( isNotNullP( rdir_peer))
            {
                rdir_peer->setPeerInDir( this, dir);
            }
            p->setPeerInDir( this, rdir);
            setPeerInDir( rdir_peer, rdir);
        }
    }
    
    /** Attach in default direction */
    inline void attach( MListItem< 1>* peer)
    {
        attachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Detach from neighbours */
    inline void detach()
    {
        /** Correct links in peers */
        if ( isNotNullP( peer[ LIST_DIR_DEFAULT]))
        {
            peer[ LIST_DIR_DEFAULT]->setPeerInDir( peer[ LIST_DIR_RDEFAULT], LIST_DIR_RDEFAULT);
        }
        if ( isNotNullP( peer[ LIST_DIR_RDEFAULT]))
        {
            peer[ LIST_DIR_RDEFAULT]->setPeerInDir( peer[ LIST_DIR_DEFAULT], LIST_DIR_DEFAULT);
        }
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }

    /** Default constructor */
    MListItem()
    {
        zeroLinks();
    };

    /** Insert element before the given one */
    MListItem( MListItem< 1> *peer)
    {
        zeroLinks();
        attachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Insert element in given direction */
    MListItem( MListItem< 1> *peer, ListDir dir)
    {
        zeroLinks();
        attachInDir( peer, dir);
    }

    /** Destructor */
    virtual ~MListItem()
    {
        detach();
    }
};

/** Short name for simple list item */
typedef MListItem< 1> SListItem;

/**
 * Specialization of interface for simple list
 * @ingroup List
 */
template< class Item, class ListBase> class MListIface< Item, ListBase, 1>: public ListBase
{
public:
    /** Return next item in default direction */
    inline Item *next() const
    {
        return static_cast< Item *>( SListItem::next());
    }
    /** Return prev item in default direction */
    inline Item* prev() const
    {
        return static_cast< Item *>( SListItem::prev());
    }
    /** Insert element before the given one */
    inline MListIface(): ListBase(){};
    /** Insert element before the given one */
    inline MListIface( Item *peer):
        ListBase( peer){};

    /** Insert element in given direction */
    inline MListIface( Item *peer, ListDir dir):
        ListBase( peer, dir){};
};

/**
 * Specialization of interface for simple list
 * @ingroup List
 */
template< class Item, class ListBase> class SListIface: public ListBase
{
public:
    /** Return next item in default direction */
    inline Item *next() const
    {
        return static_cast< Item *>( SListItem::next());
    }
    /** Return prev item in default direction */
    inline Item* prev() const
    {
        return static_cast< Item *>( SListItem::prev());
    }
    /** Insert element before the given one */
    inline SListIface():
        ListBase(){};

    /** Insert element before the given one */
    inline SListIface( Item *peer):
        ListBase( peer){};

    /** Insert element in given direction */
    inline SListIface( Item *peer, ListDir dir):
        SListItem( peer, dir){};
};

#endif