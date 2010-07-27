/**
 * @file: mem_fixed_pool.h 
 * Implementation of memory pool with fixed entry size
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_FIXED_POOL_H
#define MEM_FIXED_POOL_H


namespace Mem
{
    /**
     * Memory pool with fixed-size chunks
     * @ingroup Mem
     */
    template < class Data> 
    class FixedPool: public Pool
    {
        static const size_t CHUNK_SIZE = sizeof( MemImpl::Chunk< Data>) 
            + sizeof( MemImpl::Entry< Data>) * MemImpl::MAX_CHUNK_ENTRIES_NUM;
    public:
        /** Create fixed pool with default parameters */
        FixedPool();
        
        /** Destroy the pool */
        ~FixedPool();
                
        /** Allocate new memory block */
        void* allocate( size_t size);
        /** Free memory block */
        void deallocate( void *ptr);
        /** Functionality of 'operator delete' for pooled objects */
        void destroy( void *ptr); 
    private:        
        /** Number of used entries */
        EntryNum entry_count;
        /** First chunk */
        MemImpl::Chunk< Data> *first_chunk;
        /** First free chunk */
        MemImpl::Chunk< Data> *free_chunk;

        /* Internal routines */
        
        /** Allocate one chunk */
        inline MemImpl::Chunk< Data> *allocateChunk();
        /** Deallocate one chunk */
        inline void deallocateChunk( MemImpl::Chunk< Data> *chunk);
    };

    /** Create fixed pool with default parameters */
    template < class Data> 
    FixedPool<Data>::FixedPool(): 
        entry_count( 0),
        first_chunk( NULL),
        free_chunk( NULL)
    {

    }

    /** Destroy the pool */
    template < class Data> 
    FixedPool< Data>::~FixedPool()
    {
        /** Check that all entries are freed */
        ASSERTD( entry_count == 0);
    }

    /** Allocate one chunk */
    template < class Data> 
    MemImpl::Chunk< Data> *
    FixedPool< Data>::allocateChunk()
    {
        /** We should only allocate chunk*/
        ASSERTD( isNullP( free_chunk ));
        
        /** Allocate memory for chunk */
        void *chunk_mem = 
              ( MemImpl::Chunk< Data> *) new quint8[ CHUNK_SIZE];
        MemImpl::Chunk< Data> * chunk = new ( chunk_mem) MemImpl::Chunk< Data>();

        /** Add this chunk to pool */
        chunk->attach( first_chunk);
        chunk->setNextFree( free_chunk);
        first_chunk = chunk;
        free_chunk = chunk;
        return chunk;
    }
    
    /** Deallocate one chunk */
    template < class Data> 
    void
    FixedPool<Data>::deallocateChunk( MemImpl::Chunk< Data> *chunk)
    {
        
    }

    /** Allocate new memory block */
    template < class Data> 
    void* 
    FixedPool<Data>::allocate( size_t size)
    {
        ASSERTD( sizeof( Data) == size);
        void *ptr = NULL;
        /** If we don't have a free chunk */
        if ( isNullP( free_chunk))
        {
            /** We need to create new chunk */
            allocateChunk();
        } 
        ASSERTD( free_chunk->isFree());
        /** allocate one entry */
        ptr = free_chunk->allocateEntry();
        /** if no more entries left */
        if ( !free_chunk->isFree())
        {
            free_chunk = free_chunk->nextFree();
        }
        entry_count++;
        return new quint8[ size];
    }

    /** Free memory block */
    template < class Data> 
    void
    FixedPool<Data>::deallocate( void *ptr)
    {
        ASSERTD( entry_count > 0);
        entry_count--;
        delete[] ptr;
    }
    /** Functionality of 'operator delete' for pooled objects */
    template < class Data> 
    void
    FixedPool<Data>::destroy( void *ptr)
    {
        /** @{*/
        /** -# Check null pointer( in DEBUG mode) */
        ASSERTD( isNotNullP( ptr));
        
        /** -# Call destructor */
        Data *data_p = static_cast< Data *>( ptr);
        data_p->~Data();
        
        /** -# Free memory */
        deallocate( ptr);

        /** @}*/
    }
}; /* namespace Mem */
#endif /* MEM_FIXED_POOL_H */