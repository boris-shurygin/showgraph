/**
 * @file: mem.h 
 * Some experiments with memory manager
 * @defgroup Mem Memory Manager
 *
 * Implementation of memory manager, In Process of Design&Implementation.
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

#include "mem_ref.h" /** Memory reference */
#include "mem_obj.h" /** Memory object base class */
#include "mem_pool.h" /** Memory pool */

#endif /* MEM_H */
