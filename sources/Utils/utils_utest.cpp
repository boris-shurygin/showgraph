/**
 * @file: utils_utest.cpp 
 * Implementation of testing of memory manager
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"

class A;
typedef Single< A> SingleA;

/** Sample class */
class A
{
private:
    /** private constructors, assignment and destructor */
    A(){};
    A( const A&){};
    A& operator =( const A&){};
    ~A(){};
    /** Needed for singleton creation */
    friend SingleA;
};
/** instantiation of singleton */
SINGLETON_INSTANCE( A);    

/**
 * Test simple singleton
 */
bool uTestSingle()
{
    SingleA::init();

    A* a1 = SingleA::instance();
    A* a2 = SingleA::instance();
    ASSERT( areEqP( a1, a2));
    SingleA::deinit();
    return true;
}

/**
 * Test Utils package
 */
bool uTestUtils()
{
    /** test singleton */
    if ( !uTestSingle())
        return false;

    /** Tets list classes */
    if ( !uTestList())
        return false;

    /** Test memory management */
    if ( !uTestMem())
		return false;

    /** Test configuration classes functionality */
    if ( !uTestConf())
    	return false;
	return true;
}
