/**
 * @file: utils_utest.cpp 
 * Implementation of testing of memory manager
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"


/** Testing class for singleton */
class classA;

/** Singleton derived from class A */
typedef Single< classA> SingleA;

/** Sample class */
class classA
{
private:
    /** private constructors, assignment and destructor */
    classA(){};
    classA( const classA&){};
    classA& operator =( const classA&){};
    ~classA(){};
    /** Needed for singleton creation */
    friend class Single< classA>;
};
/** instantiation of singleton */
SINGLETON_INSTANCE( classA);    

/**
 * Test simple singleton
 */
bool uTestSingle()
{
    SingleA::init();

    classA* a1 = SingleA::instance();
    classA* a2 = SingleA::instance();
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
