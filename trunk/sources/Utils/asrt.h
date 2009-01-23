#ifndef ASRT_H
#define ASRT_H

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

template<class Excpt> inline void Assert( bool asrt, Excpt e)
{
    if ( !asrt)
    {
        throw e;
    }
}
#endif