/**
 * File: main.cpp - Entry point for GUI part
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_iface.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphW graph;
    NodeW *n =  graph.NewNode();
    n->setPos( QPoint( 0,0));
    graph.show();
    return app.exec();
}