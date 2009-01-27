/**
 * File: graph_error.cpp - Implementation of error-handling related functionality of
 * Graph library, an internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_error.h"

/**
 * TODO: Print message corresponding to error
 */
void GraphError::PrintMessage()
{

}

/**
 * Default assert in Graph library
 */
void GraphAssert( bool asrt)
{
    Assert<GraphError> (asrt);
}

/**
 * Assert throwing error description for possible handling
 */
void GraphAssert( bool asrt, GraphError e)
{
    Assert<GraphError> (asrt, e);
}