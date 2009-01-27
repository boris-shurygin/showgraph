/**
 * File: UnitTest/main.cpp - Implementation and entry point for unit testing of ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"

int main()
{
    //Test graph package
    if ( !UTestGraph())
        return -1;
}