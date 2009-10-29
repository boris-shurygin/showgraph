/**
 * @file: mem_pool.h 
 * Implementation of memory pool
 */
#pragma once

#ifndef MEM_H
#	error
#endif

#ifndef MEM_POOL_H
#define MEM_POOL_H

namespace Mem
{
    /**
     * Type for  memory entry size  
     * @ingroup Mem
     */
    typedef unsigned int EntrySize;

    /** 
     * Type for number of memory entries 
     * @ingroup Mem
     */
    typedef unsigned int EntryNum;

    /**
     * Pool types
     * @ingroup Mem
     */
    enum PoolType
    {
        /** Fixed-size entry pool */
        POOL_FIXED,
        /** Float-size entry pool */
        POOL_FLOAT,
        /** Number of types */
        POOL_TYPES_NUM
    };

    /**
     * Base class for memory pools
     *
     * @ingroup Mem
     */
    class Pool
    {
        /** Types of the pool */
        PoolType tp;
    public:
        /** Create pool, by default it is pool of fixed-size entries */
        inline Pool(): tp( POOL_FIXED){};
        /** Create pool of given type */
        inline Pool( PoolType t): tp( t){};
    };

    /**
     * Memory pool with fixed-size chunks
     * @ingroup Mem
     */
    class FixedPool: Pool
    {
        /** Size of one entry */
        EntrySize entry_size; 
        
        /** Number of used entries */
        EntryNum entry_count;
    public:
        
        /** Create fixed pool with default parameters */
        FixedPool();
        
        /** Destroy the pool */
        ~FixedPool();

        /** Create new entry */
        inline Obj * newEntry( EntrySize sz)
        {
            /** -# Check size parameter */
            assertd( sz > 0);
            assertd( sz >= sizeof( Obj));
            if ( entry_size == 0)
            {
                entry_size = sz;
            }
            assertd( sz == entry_size);
            
            /** -# Increase entry count */
            entry_count++;

            /** -# STUB: simply calling malloc instead of intended pool logic */
            return ( Obj* )malloc( sz);
        }

        /** Free entry */
        inline void freeEntry( Obj * entry)
        {
            /** @{ */
            /** -# Check entry count */
            assertd( entry_count != 0);
            
            /** -# Decrease entry count */
            entry_count--;
            
            /** -# STUB: simply calling free instead of intended pool logic */
            free( entry);
            
            /** @} */
        }
    };
}; /* namespace Mem */
#endif /* MEM_POOL_H */