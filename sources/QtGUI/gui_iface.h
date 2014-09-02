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
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

/**
 * Debug assert for graph library
 */
#if !defined(GUI_ASSERTD)
#    define GUI_ASSERTD(cond, what) ASSERT_XD(cond, "GUI", what)
#endif

#include "../Core/GraphView/gview_iface.h"
#include "../Frontend/fe_iface.h"
#include "../IR/ir_iface.h"
#include "text_view.h"
#include "main_window.h"

/** Gui project uses Qt versions 4.4 and higher */
#if ( QT_VERSION < QT_VERSION_CHECK(4, 4, 0) )
#  error "Please use Qt version 4.4 and higher"
#endif

#endif