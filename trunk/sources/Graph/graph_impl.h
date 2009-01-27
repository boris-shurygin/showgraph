/**
 * File: graph_impl.h - Implementational header for modules of Graph library, 
 * an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_IMPL_H
#define GRAPH_IMPL_H

/**
 * Graphs interfaces
 */
#include "graph_iface.h"

/** 
 * Implementation-required headers
 */
#include "../Utils/utils_iface.h"
#include "graph_error.h"

/** 
 * namespaces import
 */
using namespace Utils;

/**
 * Return direction that is reverse to given one
 */
inline GraphDir
RevDir( GraphDir dir)
{
    GraphAssert( GRAPH_DIRS_NUM == 2);
    return ( dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN: GRAPH_DIR_UP; 
}

#endif
