/**
 * @file: ir_iface.h 
 * Interface of Showgraph IR package
 * @defgroup IR Intermediate representation
 * @author Boris Shurygin
 *
 * Intermediate representation keeps information of the visualized data structures such as
 * control flow and data flow graphs, it can also have some analytical data like
 * dominator/posdominator and loop trees
 */
/*
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef IR_IFACE_H
#define IR_IFACE_H

/** Predeclarations */
class CFG;
class IR;
class IRView;
class CFEdge;
class CFNode;
class Dep;
class Expr;

#include "../QtGUI/gui_iface.h"
#include "cfedge.h"
#include "cfnode.h"
#include "cfg.h"
#include "dep.h"
#include "expr.h"
#include "ir.h"

#endif /* IR_IFACE_H */
