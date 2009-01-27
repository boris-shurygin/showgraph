/**
 * File: misc.h - Various auxiliary routines for ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef MISC_H
#define MISC_H

/**
 * If pointer is not null
 */
inline bool IsNotNullP( void *pointer)
{
    return pointer != NULL;
}
/**
 * If pointer is null
 */
inline bool IsNullP( void *pointer)
{
    return pointer == NULL;
}

#endif