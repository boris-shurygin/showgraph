/**
 * @file: singleton.h 
 * @defgroup Singleton Singleton
 * @brief Definition of singleton template
 * @ingroup Utils
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
#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * @brief Template for simple singleton objects.
 * @ingroup Singleton
 * @param T type of client object
 *
 * Singleton is a type of object that is guaranteed to have only one instance at any
 * given point of program execution. The Single template manages global access to single
 * instance of client object. Client object type is specified by template parameter. 
 * A pointer to the single instance of client object can be obtained by calling intance().
 * To control singleton's lifetime its initialization and destruction are made explicit.
 * Use init() and deinit() routines for these purposes.
 * Example of usage:
 @code
 // Sample class
 class classA
 {
 public:
     doSomething() {...}
 private:
     // private constructors, assignment and destructor
     classA(){};
     classA( const classA&){};
     classA& operator =( const classA&){};
     ~classA(){};
     // Needed for singleton opration
     friend class Single< classA>;
 };
 
 // Typedef for classA encapsulated in singleton
 typedef Single< classA> SingleA;

 //void myfunc()
 {
    SingleA::instance()->doSomething();
 }
 // Usage
 void useSingleA()
 {
    SingleA::init();
    myfunc();
    SingleA::deinit();
 }
 @endcode
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
