/**
 * File: utest_impl.h - Header for unit testing of libraries and tools of ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#include <stdio.h>
#include <QRegExp>
#include "../Graph/graph_iface.h"
#include "../Utils/utils_iface.h"
#include "../QtGUI/gui_iface.h"
#include "../Frontend/fe_iface.h"

/**
 * Test list classes
 */
bool uTestList();

/**
 * Test memory manager
 */
bool uTestMem();

/**
 * Test configuration-related functionality
 */
bool uTestConf();

/**
 * Graph library unit testing
 */
bool uTestGraph();

/**
 * UnitTesting of GUI
 */
bool uTestGUI(int argc, char **argv);

/**
 * Frontend unit testing
 */
bool uTestFE();
