/**
 * @file: mem_pool.cpp 
 * Implementation of memory pool
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include "mem.h"

using namespace Mem;

/** Create fixed pool with default parameters */
FixedPool::FixedPool(): Pool( POOL_FIXED), entry_count( 0)
{

}

/** Destroy the pool */
FixedPool::~FixedPool()
{
    /** Check that all entries are freed */
    assertd( entry_count == 0);
}
