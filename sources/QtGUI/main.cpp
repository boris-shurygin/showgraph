/**
 * File: main.cpp - Entry point for GUI part
 * Copyright (C) 2009  Boris Shurygin
 */
#include <QtGui>
#include "graph_w.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphW graph;
    graph.show();
    return app.exec();
}