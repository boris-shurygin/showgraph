/**
 * @file: mem.h 
 * Some experiments with memory manager
 * @defgroup Mem Memory Manager
 *
 * Implementation of memory manager. In Process of Design.
 *
 * Memory manager should solve the following memory-related problems
 * - Dangling pointers. Solution: reference counting via smart pointers
 * - Memory leaks. Solution: Various checks in pools and in pointers
 * - Fragmentation. Solution: Fixed-sized chunks used in pools
 * - Poor locality. Solution: not clear yet :(
 * @ingroup Utils
 */
/*
 * Utils library in Showgraph tool
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#define MEM_H

#ifdef _DEBUG
#  define USE_REF_COUNTERS
#endif

/**
 * Namespace for memory manager
 * @ingroup Mem
 */
namespace Mem
{
   /**
    * Base class for all memory-managed objects
    * @ingroup Mem
    */
    class Obj
    {
#ifdef USE_REF_COUNTERS

#endif
    };

    /**
     * Smart pointer template
     * @ingroup Mem
     */
     template < class RefObj> class Ref
     {
     
     };

     /**
      * Template of memory pool with fixed-size chunks
      * @ingroup Mem
      */
     template < class Data> class FixedPool
     {
     
     };

}; /* namespace Mem */
#endif /* MEM_H */
