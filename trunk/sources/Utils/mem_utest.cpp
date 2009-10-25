/**
 * @file: graph_utest.cpp 
 * Implementation of testing of Graph library
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include "mem.h"

using namespace Mem;

/** Test object class */
class TestObj: public Obj
{
	public:	
		int a;
};

/**
 * Reference to object
 */
typedef Ref< TestObj> ObjRef;

/**
 * Test smart pointers, objects and pools
 */
bool uTestMem()
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
	assertd( ref->refCount() == 2);
	
	bool catched = false;
	
	/** Test exception generation */
	try
	{
		delete ref;
	} catch( int a)
	{
		catched = true;
	}
	assertd( catched);
	
	/** Test ref to pointer conversion and Obj destructor */
	delete ref2;

	return true;
}
