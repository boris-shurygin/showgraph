/**
 * @file: singleton.h 
 * Definition of singleton template
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * Template for simple singleton objects
 */
template < class T> class Single
{
    /** Pointer to client object */
    static T* instance_p; // MUST be assigned to zero by client code
public:
    /** Create and initialize client object */
    static void init();
    /** Destroy client object */
    static void deinit();
    /** Get a pointer to client object */
    static T* instance();
private:
    /** Uniqueness ensurance */
    /** Private constructor */
    Single();
    /** Private copy constructor */
    Single( const Single&);
    /** Private assignment */
    Single& operator = ( const Single&);
    /** Private destructor */
    ~Single();
};

/**
 * Initializtion of singleton
 */
template < class T> 
void
Single< T>::init()
{
    ASSERTD( isNullP( instance_p));
    instance_p = new T();
}
/**
 * Destruction of singleton
 */
template < class T> 
void
Single< T>::deinit()
{
    ASSERTD( isNotNullP( instance_p));
    delete instance_p;
    instance_p = 0;
}
/**
 * Access to client object
 */
template < class T> 
T*
Single< T>::instance()
{
    ASSERTD( isNotNullP( instance_p));
    return instance_p;
}

/**
 * Access to client object
 * is implemented as a macro for using in client .cpp file
 * WARNING: without calling this macro singleton object won't work
 */
#define SINGLETON_INSTANCE(CLIENT_CLASS)   \
template <> \
CLIENT_CLASS* Single<CLIENT_CLASS>::instance_p = 0;                                       



#endif /* SINGLETON_H */
