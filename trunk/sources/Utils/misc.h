#ifndef MISC_H
#define MISC_H

inline bool IsNotNullP( void *pointer)
{
    return pointer != NULL;
}

inline bool IsNullP( void *pointer)
{
    return pointer == NULL;
}

#endif