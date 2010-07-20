/**
 * @file: mem_chunk.h 
 * Implementation of memory chunk
 */
/*
 * Utils/Mem library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_CHUNK_H
#define MEM_CHUNK_H

namespace MemImpl
{
    /**
     * Class to control memory chunks
     */
    template<class Data> class Chunk: public SListItem
    {
        /** Pointer to next free chunk */
        Chunk<Data> *next_free;
        /** Get next free chunk */
        inline Chunk< Data> *nextFree() const;
        /** Set Next free chunk */
        inline void setNextFree( Chunk< Data> *ch);
    };

    /** Get next free chunk */
    template<class Data> 
    Chunk< Data>*
    Chunk< Data>::nextFree() const
    {
        return next_free;
    }

    /** Set next free chunk */
    template<class Data> 
    void
    Chunk< Data>::setNextFree( Chunk< Data> *ch)
    {
        next_free = ch;
    }
}; /* namespace MemImpl */
#endif /* MEM_CHUNK_H */