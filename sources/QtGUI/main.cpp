/**
 * File: main.cpp - Entry point for GUI part
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_iface.h"
#include <QLineEdit>
#include <QMainWindow>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphW graph;
    graph.show();
    return app.exec();
}