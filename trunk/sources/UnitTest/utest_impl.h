/**
 * File: utest_impl.h - Header for unit testing of libraries and tools of ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#include <stdio.h>
#include <QRegExp>
#include "../Core/core_iface.h"
#include "../QtGUI/gui_iface.h"
#include "../Frontend/fe_iface.h"

/**
 * Test Utils package
 */
bool uTestUtils();

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
