/**
 * File: gui_iface.h - Interface of GUI project
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

/** Predeclarations */
class NodeItem;
class EdgeItem;
class GraphView;

#include "../Graph/graph_iface.h"
#include "../Layout/layout_iface.h"
#include <QtGui>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QWheelEvent>

enum { 
    TypeNode = QGraphicsItem::UserType + 1,
    TypeEdge = QGraphicsItem::UserType + 2,
    TypeEdgeControl = QGraphicsItem::UserType + 3
};

#include "node_w.h"
#include "edge_w.h"
#include "graph_w.h"
#include "main_window.h"


#endif