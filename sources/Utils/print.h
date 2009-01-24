#ifndef PRINT_H
#define PRINT_H

#include <cstdio>
#include <cstdarg>

namespace PrintUtils
{
    /** Print to file */
    
    /** Print to console's STDOUT */
    inline void out( const char* format, ...)
    {
        va_list args;
        va_start( args, format);
        vfprintf( stdout, format, args);
        va_end( args);
        fprintf( stderr, "\n");
    }
    /** Print to console's STDERR */
    inline void err( const char* format, ...)
    {
        va_list args;
        va_start( args, format);
        vfprintf( stderr, format, args);
        va_end( args);
        fprintf( stderr, "\n");
    }
};

#endif