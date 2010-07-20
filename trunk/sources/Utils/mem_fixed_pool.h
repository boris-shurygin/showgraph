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
        /** Number of used entries */
        EntryNum entry_count;
        /** First chunk */
        MemImpl::Chunk< Data> *first_chunk;
        /** First free chunk */
        MemImpl::Chunk< Data> *free_chunk;
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

    /** Allocate new memory block */
    template < class Data> 
    void* 
    FixedPool<Data>::allocate( size_t size)
    {
        ASSERTD( sizeof( Data) == size);
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