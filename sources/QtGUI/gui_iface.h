/**
 * @file: gui_iface.h 
 * Interface of Showgraph GUI package
 * @defgroup GUI Graphical User Interface 
 *
 * Graphical part of ShowGraph is arranged around MainWindow wich has 
 * GraphView as a center widget.
 */
/**
 * GUI implementation.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

/** Predeclarations */
class GGraph;
class GNode;
class GEdge;
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

#include "../Graph/graph_iface.h"
#include "../Layout/layout_iface.h"
#include "../Frontend/fe_iface.h"

/**
 * Subclasses of QGraphicsItem types
 */
enum { 
    TypeNode = QGraphicsItem::UserType + 1, /** Node item */
    TypeEdge = QGraphicsItem::UserType + 2, /** Edge item */
    TypeEdgeControl = QGraphicsItem::UserType + 3 /** EdgeControl item */
};

#include "edge_item.h"
#include "node_item.h"
#include "graph_view.h"
#include "main_window.h"
#include "graph_widget.h"
#include "text_view.h"


#endif