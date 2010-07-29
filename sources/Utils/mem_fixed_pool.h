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
        /** Get pointer to chunk from pointer to entry */
        inline MemImpl::Chunk< Data> *entryChunk( MemImpl::Entry< Data> *e);
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
        /** Deallocated cached chunks */
        while ( isNotNullP( first_chunk))
        {
            deallocateChunk( first_chunk);
        }
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
        
#ifdef CHECK_CHUNKS
        chunk->pool = ( void *)this;
#endif
        return chunk;
    }
    
    /** Deallocate one chunk */
    template < class Data> 
    void
    FixedPool<Data>::deallocateChunk( MemImpl::Chunk< Data> *chunk)
    {
#ifdef CHECK_CHUNKS
        ASSERTD( chunk->isEmpty());
        ASSERTD( areEqP( chunk->pool, this));
#endif
        if ( areEqP( first_chunk, chunk))
        {
            first_chunk = chunk->next();
        }
        chunk->~Chunk(); 
        delete[] ( void *)chunk;
    }

    /** Calculate pointer to chunk from pointer to entry */
    template < class Data> 
    MemImpl::Chunk< Data> *
    FixedPool<Data>::entryChunk( MemImpl::Entry< Data> *e)
    {
        MemImpl::ChunkPos e_pos = e->pos();
        quint8 *ptr = ( quint8 *) e;
        ptr = ptr - sizeof( MemImpl::Entry< Data>) * e_pos - sizeof ( MemImpl::Chunk< Data>);
        return (MemImpl::Chunk< Data> *) ptr;
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
        ptr = ( void *)free_chunk->allocateEntry();
        /** if no more entries left */
        if ( !free_chunk->isFree())
        {
            free_chunk = free_chunk->nextFree();
        }
        entry_count++;
        return ptr;
    }

    /** Free memory block */
    template < class Data> 
    void
    FixedPool<Data>::deallocate( void *ptr)
    {
        ASSERTD( entry_count > 0); // Check entry count

        MemImpl::Entry< Data> *e =(MemImpl::Entry< Data> *) ptr;
        MemImpl::Chunk< Data> *chunk = entryChunk( e);

#ifdef CHECK_CHUNKS
        // Check that we are deleting entry from this pool
        ASSERT( areEqP( this, chunk->pool)); 
#endif
        /**
         * If chunk is free already - it must be in free list 
         * no need to add it again
         */
        bool add_to_free_list = !chunk->isFree();

        chunk->deallocateEntry( e);
        
        if ( add_to_free_list
             && chunk != free_chunk)
        {
            if ( isNotNullP( free_chunk) && free_chunk->isEmpty())
            {
                chunk->setNextFree( free_chunk->nextFree());
                deallocateChunk( free_chunk);
            } else
            {
                chunk->setNextFree( free_chunk);
            }
            free_chunk = chunk;
        }
        entry_count--;
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