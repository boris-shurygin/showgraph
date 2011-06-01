/**
 * @file: gui_iface.h 
 * Interface of Showgraph GUI package
 * @defgroup GUI Graphical User Interface 
 * @author Boris Shurygin
 *
 * Graphical part of ShowGraph is arranged around MainWindow wich has 
 * GraphView as a center widget.
 */

/**
 * @mainpage
 *
 * @par Graph Editor
 * Showgraph tool is a simple graph editor/viewer. It allows for creating nodes 
 * and connecting them with edges. Nodes have ediatable labels and edges can be
 * routed using control points. One of primary goals was to create lightweight and
 * easy-to-use tool for fast construction of a graph's visual representation. 
 * Nodes are simply created by double-click on background and edges are created by 
 * pressing right mouse button on one node, holding it down while moving to other node
 * and finally releasing it on the target node. Nodes can be moved around by simply
 * dragging them. See @subpage iface section for more details.
 *
 * @par Graph Visualization
 * Showgraph also features an auto-layout for directed graphs. Once you have constructed
 * graph you can use automatic layout to provide a basic structured view that can be further
 * adjusted manually to produce nicer image of the graph.
 * See @subpage layout section for info on automatic graph drawing.
 *
 * @par Import/Export
 * Constructed graphs are stored in XML files. Graph images can be also exported in various
 * formats or sent to printer.
 */
/**
 * GUI implementation.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

/**
 * Debug assert for graph library
 */
#if !defined(GUI_ASSERTD)
#    define GUI_ASSERTD(cond, what) ASSERT_XD(cond, "GUI", what)
#endif

#include "gview_iface.h"
#include "../Frontend/fe_iface.h"
#include "../IR/ir_iface.h"
#include "text_view.h"
#include "main_window.h"

/** Gui project uses Qt versions 4.4 and higher */
#if ( QT_VERSION < QT_VERSION_CHECK(4, 4, 0) )
#  error "Please use Qt version 4.4 and higher"
#endif

#endif