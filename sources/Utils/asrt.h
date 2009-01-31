/**
 * File: misc.h - Assertion related routines of Utils library for ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef ASRT_H
#define ASRT_H

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
template<class Excpt> inline void Assert( bool assertion)
{
    if ( !assertion)
    {
        throw Excpt();
    }
}

/**
 * Assertion template parameterized with thrown exception type
 */
template<class Excpt> inline void Assert( bool asrt, Excpt e)
{
    if ( !asrt)
    {
        throw e;
    }
}

/**
 * Generic assert
 */
inline void Assert( bool asrt)
{
    return Assert< int>( asrt);
}
#endif
