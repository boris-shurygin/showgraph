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
    /** Chunk lists identificators */
    enum ChunkListType
    {
        CHUNK_LIST_ALL,
        CHUNK_LIST_FREE,
        CHUNK_LISTS_NUM
    };

    /**
     * Class to control memory chunks
     */
    template<class Data> class Chunk: 
        public MListIface< Chunk< Data>, // List client data
                           MListItem< CHUNK_LISTS_NUM>, // base class: pure multi-list item
                           CHUNK_LISTS_NUM > // Lists number
    {
        /** position of first free entry */
        ChunkPos free_entry;
        /** busy entries num */
        ChunkPos busy;
        /** Get chunk for given number */
        inline Entry<Data> *entry( ChunkPos pos);

    public:
#ifdef CHECK_CHUNKS
        void *pool;
#endif  
        /** Constructor */
        inline Chunk();
        /** Check if this chunk has free entries */
        inline bool isFree() const;
        /** Check if this chunk is empty */
        inline bool isEmpty() const;
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
        busy = 0;
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
   
    /** Check if this chunk has free entries */
    template<class Data> 
    bool 
    Chunk< Data>::isFree() const
    {
        return free_entry != UNDEF_POS;
    }
    /** Check if this chunk is empty */
    template<class Data> 
    bool 
    Chunk< Data>::isEmpty() const
    {
        return busy == 0;
    }      
    /** Allocate one entry */
    template<class Data> 
    Data*
    Chunk< Data>::allocateEntry()
    {
        ASSERTD( this->isFree());
        
        Entry< Data> *e = entry( free_entry);
#ifdef USE_MEM_EVENTS        
        MemMgr::instance()->allocEvent();
#endif        
        Data *res = static_cast<Data *>( e);
        free_entry = e->nextFree();
        busy++;
        return res;
    }
    /** Deallocate one entry */
    template<class Data> 
    void
    Chunk< Data>::deallocateEntry( Entry<Data> *e)
    {
        ASSERTD( busy > 0);
#ifdef USE_MEM_EVENTS        
        MemMgr::instance()->deallocEvent();
#endif 
        e->setNextFree( free_entry);
        free_entry = e->pos();
        busy--;
    }
}; /* namespace MemImpl */
#endif /* MEM_CHUNK_H */