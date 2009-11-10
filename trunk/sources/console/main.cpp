/**
 * @file: main.cpp 
 * Implementation of console version of showgraph
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include <stdio.h>
#include <QRegExp>
#include "../Graph/graph_iface.h"
#include "../Utils/utils_iface.h"
#include "../QtGUI/gui_iface.h"
#include "../Frontend/fe_iface.h"

int main(int argc, char **argv)
{
    Conf conf;
    conf.addOption( new Option( OPT_STRING, "f", "file", "input graph description file name"));
    conf.addOption( new Option( OPT_STRING, "o", "output", "output image file name"));
    conf.readArgs( argc, argv);
    conf.printOpts(); // Print options to console
}