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
    template<class Data> class Chunk: public SListIface< Chunk< Data>>
    {
        /** Pointer to next free chunk */
        Chunk<Data> *next_free;
        /** position of first free entry */
        ChunkPos free_entry;
        /** Get chunk for given number */
        inline Entry<Data> *entry( ChunkPos pos);
    public:
        /** Constructor */
        inline Chunk();
        /** Get next free chunk */
        inline Chunk< Data> *nextFree() const;
        /** Set Next free chunk */
        inline void setNextFree( Chunk< Data> *ch);    
        /** Check if this chunk has free entries */
        inline bool isFree() const;
        /** Initialization of chunk */
        inline void initialize();
        /** Allocate on entry */
        inline Data *allocateEntry();
        /** Deallocate one entry */
        inline void deallocateEntry( Entry<Data> *e);
        /** Placement new */
        inline void *operator new ( size_t size, void* mem);
        /**
         * Operator 'delete' corresponding to placement new
         * WARNING: Compiler won't call this for deletion. 
         *          It is needed for freeing memory in case of exceptions in constructor
         */
        inline void operator delete( void *ptr, void* mem);
    };
    
    /** Constructor */
    template <class Data> 
    Chunk< Data>::Chunk()
    {
        Entry<Data> *e = NULL;
        
        for ( int i = 0; i < MAX_CHUNK_ENTRIES_NUM; i++)
        {
            e = ( Entry< Data> *)( (quint8 *) this 
                                       + sizeof( Chunk< Data>) 
                                       + sizeof( Entry< Data>) * i);
            e->setPos( i);
            e->setNextFree( i + 1);
        }
        ASSERTD( e->nextFree() == UNDEF_POS);
        free_entry = 0;
    }
    /** Placement new */
    template <class Data> 
    void*
    Chunk< Data>::operator new ( size_t size, void* mem)
    {
        return mem;
    }
    /**
     * Operator 'delete' corresponding to placement new
     * WARNING: Compiler won't call this for deletion. 
     *          It is needed for freeing memory in case of exceptions in constructor
     */
    template <class Data> 
    void
    Chunk< Data>::operator delete( void *ptr, void* mem)
    {
    
    }
  
    /** Get entry by number */
    template<class Data> 
    Entry< Data>*
    Chunk< Data>::entry( ChunkPos pos)
    {
        ASSERTD( pos != UNDEF_POS);
        return ( Entry< Data> *)( (quint8 *) this 
                                  + sizeof( Chunk< Data>) 
                                  + sizeof( Entry< Data>) * pos);
    }
   
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
    /** Check if this chunk has free entries */
    template<class Data> 
    bool 
    Chunk< Data>::isFree() const
    {
        return free_entry != UNDEF_POS;
    }
        
    /** Allocate one entry */
    template<class Data> 
    Data*
    Chunk< Data>::allocateEntry()
    {
        ASSERTD( this->isFree());

        return static_cast<Data *>( entry( free_entry));
    }
    /** Deallocate one entry */
    template<class Data> 
    void
    Chunk< Data>::deallocateEntry( Entry<Data> *e)
    {
        free_entry = e->pos();
        e->setNextFree( free_entry);
    }
}; /* namespace MemImpl */
#endif /* MEM_CHUNK_H */