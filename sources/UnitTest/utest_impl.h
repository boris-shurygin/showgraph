/**
 * File: utest_impl.h - Header for unit testing of libraries and tools of ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#include <stdio.h>
#include "../Graph/graph_iface.h"
#include "../Utils/utils_iface.h"
#include "../QtGUI/gui_iface.h"

/**
 * Graph library unit testing
 */
bool UTestGraph();

/**
 * UnitTesting of GUI
 */
bool UTestGUI(int argc, char **argv);
