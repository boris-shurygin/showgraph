#ifndef ASRT_H
#define ASRT_H

enum ErrorT
{
    ERROR_GENERIC
};
enum Severity
{
    CRITICAL
};


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

template<class Excpt> inline void Assert( bool asrt, Excpt e)
{
    if ( !asrt)
    {
        throw e;
    }
}
#endif