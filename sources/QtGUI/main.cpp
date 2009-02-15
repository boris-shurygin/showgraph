/**
 * File: main.cpp - Entry point for GUI part
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_iface.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphW graph;
    NodeW *n1 =  graph.NewNode();
    NodeW *n2 =  graph.NewNode();
    n1->setPos( QPoint( 10,0));
    n2->setPos( QPoint( -10,0));
    EdgeW* edge = graph.NewEdge( n1, n2);
    graph.show();
    return app.exec();
}