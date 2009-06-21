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

#include <QtGui>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QWheelEvent>

const qreal RANK_SPACING = 60;

#include "../Graph/graph_iface.h"
#include "../Layout/layout_iface.h"

enum { 
    TypeNode = QGraphicsItem::UserType + 1,
    TypeEdge = QGraphicsItem::UserType + 2,
    TypeEdgeControl = QGraphicsItem::UserType + 3
};

#include "node_item.h"
#include "edge_item.h"
#include "graph_view.h"
#include "main_window.h"


#endif