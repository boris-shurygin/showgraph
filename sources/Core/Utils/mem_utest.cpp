/**
 * @file: mem_utest.cpp 
 * Implementation of testing of memory manager
 */
/*
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "utils_iface.h"
#include "mem.h"

using namespace Mem;

/** Test object class */
class TestObj: public Obj
{
	
public:	
	/** Some variable */
	int a;
};

/**
 * Reference to object
 */
typedef Ref< TestObj> ObjRef;

/**
 * Test smart pointers
 */
static bool
uTestRef()
{
/** #Test smart pointers behaviour */
    ObjRef ref = new TestObj(); /** Test constructor from pointer */
    ObjRef ref2; /** Test default constructor */

    /** Test operator bool() */
    assertd( !ref2 && ref);
    assertd( ref2 == NULL && ref != NULL);
    /** Test copy constructor */
    ref2 = ref;
    assertd( ref2 && ref);
    /** Test operator == ( ref) */
    assertd( ref == ref2);

    /** Test operator -> */
    ref->a = 2;
#ifdef USE_REF_COUNTERS
    assertd( ref->refCount() == 2);
#endif	
    bool catched = false;

    /** Test exception generation */
    try
    {
            delete ref;
    } catch( int a)
    {
            catched = true;
            ref2->a = a;
    }
    assertd( catched);

    /** Test ref to pointer conversion and Obj destructor */
    delete ref2;
    return true;
}

/** Sample object used as a baseclass for more complicated pool-stored objects */
class PoolBase: public PoolObj 
{
    virtual void setVal( quint32 val) = 0;
    virtual quint32 val() const = 0;
};

/**
 * @brief Fairly complex pool-stored object
 * Class for testing complex objects stored in pool. Built with 
 * multiple inheritance, virtual functions and additional members.
 */
class MyPoolObj: public PoolBase, public SListIface< MyPoolObj, SListItem>
{
    quint32 priv_field;
public:
    /** Some public fields */
    quint32 a;
    quint32 b;
    bool *called;
    /** Base class routines implementation */
    void setVal( quint32 val)
    {
        priv_field = val;
    }
    quint32 val() const
    {
        return priv_field;
    }
    ~MyPoolObj()
    {
        *called = true;
    }
};
/**
 * Test memory pools
 */
static bool
uTestPools()
{

    Pool *pool = new FixedPool< MyPoolObj>();
    MyPoolObj *p1 = new ( pool) MyPoolObj();
    MyPoolObj *p2 = new ( pool) MyPoolObj();
    bool called_destructor1 = false;
    bool called_destructor2 = false;

    ASSERT( p1 != p2);
    p1->a = 1;
    p2->a = 2;
    p1->b = 3;
    p2->b = 4;
    p1->called = &called_destructor1;
    p2->called = &called_destructor2;

    p1->setVal( 5);
    p2->setVal( 6);
    ASSERT( p1->a != p1->b);
    ASSERT( p1->a != p2->a);
    ASSERT( p1->b != p2->a);
    ASSERT( p1->b != p2->b);
    ASSERT( p1->a != p2->a);
    
    ASSERT( !called_destructor1);
    ASSERT( !called_destructor2);
    
    pool->destroy( p1);
    
    ASSERT( called_destructor1);
    ASSERT( !called_destructor2);

    pool->destroy( p2);
    
    ASSERT( called_destructor1);
    ASSERT( called_destructor2);

    /** More objects */
    MyPoolObj *obj = NULL;
    for ( int i = 0; i < 20000; i++)
    {
        MyPoolObj *prev_obj = obj;
        obj = new ( pool) MyPoolObj();
        obj->called = &called_destructor1;
        obj->attach( prev_obj);
        prev_obj = obj->next();
    }
    while ( isNotNullP( obj))
    {
        MyPoolObj *next = obj->next();
        pool->destroy( obj);
        obj = next;
    }
    delete pool;
    return true;
}


/**
 * Test smart pointers, objects and pools
 */
bool uTestMem()
{
    /** Test smart pointers */
    if ( !uTestRef())
        return false;
    /** Test memory pools */
    if ( !uTestPools())
        return false;
    return true;
}
