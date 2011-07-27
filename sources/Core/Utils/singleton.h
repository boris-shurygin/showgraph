/**
 * @file: singleton.h 
 * @defgroup Singleton Singleton
 * @brief Definition of singleton template
 * @ingroup Utils
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * Template for simple singleton objects
 * @ingroup Singleton
 */
template < class T> class Single
{
public:
    /** Create and initialize client object */
    static void init();
    /** Destroy client object */
    static void deinit();
    /** Get a pointer to client object */
    static T* instance();

private:/* Uniqueness ensurance */
    /** Private constructor */
    Single();
    /** Private copy constructor */
    Single( const Single&);
    /** Private assignment */
    Single& operator = ( const Single&);
    /** Private destructor */
    ~Single();
    /**
     * @brief Pointer to client object.
     * 
     * Initialized to NULL on start and points to instance after init() is called 
     */
    static T* instance_p; 
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
 * Initialization of instance pointer
 */
template < class T> T *Single<T>::instance_p = 0;                                       



#endif /* SINGLETON_H */
