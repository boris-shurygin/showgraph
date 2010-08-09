/**
 * @file: asrt.h 
 * Assertion related routines of Utils library for ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef ASRT_H
#define ASRT_H

#if !defined(ASSERTD)
#  ifdef _DEBUG
#    define ASSERTD(cond) Q_ASSERT(cond)
#  else
#    define ASSERTD(cond)
#  endif
#endif

#if !defined(ASSERT_XD)
#  ifdef _DEBUG
#    define ASSERT_XD(cond, where, what) Q_ASSERT_X(cond, where, what)
#  else
#    define ASSERT_XD(cond, where, what)
#  endif
#endif

#if !defined(ASSERT)
#  define ASSERT(cond) Q_ASSERT(cond)
#endif

/**
 * Type of error
 */
enum ErrorT
{
    ERROR_GENERIC
};

/**
 * Error's severity
 */
enum Severity
{
    CRITICAL
};

/**
 * Error description
 */
class Error
{
    ErrorT type;
    Severity sev;
public:
    Error()
    {
        type = ERROR_GENERIC;
        sev = CRITICAL;
    }
};

/**
 * Generic assertion template
 */
template<class Excpt> inline void assert( bool assertion)
{
    if ( !assertion)
    {
        throw Excpt();
    }
}

/**
 * Assertion template parameterized with thrown exception type
 */
template<class Excpt> inline void assert( bool asrt, Excpt e)
{
    if ( !asrt)
    {
        throw e;
    }
}

/**
 * Generic assert
 */
inline void assert( bool asrt)
{
    assert< int>( asrt);
}

/**
 * Debug assert
 */
inline void assertd( bool asrt)
{
#ifdef _DEBUG
    assert< int>( asrt);
#endif
}

#endif /* ASRT_H */
