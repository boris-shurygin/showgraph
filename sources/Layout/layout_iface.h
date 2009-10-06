/**
 * @file: layout_iface.h 
 * Layout interface
 *
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 *
 * @defgroup Layout 2D Graph layout
 */
#ifndef LAYOUT_IFACE_H
#define LAYOUT_IFACE_H

class AuxNode;
class AuxEdge;
class AuxGraph;
class Level;

#include <QtGui>
#include <QList>

const qreal NODE_NODE_MARGIN = 30;
const qreal NODE_CONTROL_MARGIN = 5;
const qreal CONTROL_CONTROL_MARGIN = 5;
const qreal EDGE_CONTROL_WIDTH = 5;
const qreal EDGE_CONTROL_HEIGHT = 5;

const qreal RANK_SPACING = 60;

#include "../Graph/graph_iface.h"
#include "aux_graph.h"
#include "node_group.h"

#endif /** LAYOUT_IFACE_H */