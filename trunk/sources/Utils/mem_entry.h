/**
 * @file: mem_entry.h 
 * Implementation of memory entry
 */
/*
 * Utils/Mem library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_ENTRY_H
#define MEM_ENTRY_H

namespace MemImpl
{
    /**
     * Entry in memory that holds user data
     *
     * @ingroup MemImpl
     */
    template < class Data> class Entry: private Data
    {
        /** Own position of this entry in chunk */
        ChunkPos my_pos;
        /** Position of next free entry in chunk */
        ChunkPos next_free_pos;
    private:
        /** Private constructor to prevent direct creation of such objects */
        Entry();
        /** Private destructor */
        ~Entry();
        /** Get position */
        inline ChunkPos pos() const;
        /** Get position of next free chunk */
        inline ChunkPos nextFree() const;
        /** Set position */
        inline void setPos( ChunkPos pos);
        /** Set position of next free chunk */
        inline void setNextFree( ChunkPos next);
        /** Get pointer to chunk */
        inline Chunk * chunk() const;
    };

    /**
     * Private constructor to prevent direct creation of such objects
     */
    template< class Data>
    Entry< Data>::Entry()
    {
        assert( 0);
    }
    /**
     * Private destructor
     */
    template< class Data>
    Entry< Data>::~Entry()
    {
        assert( 0);
    }
    /** Get position */
    template< class Data>
    ChunkPos 
    Entry< Data>::pos() const
    {
        return my_pos;
    }
    /** Get position of next free chunk */
    template< class Data>
    ChunkPos
    Entry< Data>::nextFree() const
    {
        return next_free_pos;
    }
    /** Set position */
    template< class Data>
    void
    Entry< Data>::setPos( ChunkPos pos)
    {
        my_pos = pos;
    }
    /** Set position of next free chunk */
    template< class Data>
    void
    Entry< Data>::setNextFree( ChunkPos next)
    {
        next_free_pos = next;
    }
};

#endif /* MEM_ENTRY_H */