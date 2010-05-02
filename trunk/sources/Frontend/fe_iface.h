/**
 * @file: fe_iface.h 
 * Interface of Frontend in ShowGraph tool
 *
 * @defgroup FE Input file parsing
 * Frontend for parsing the graph dumps form various compiers
 */
/*
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef FE_IFACE_H
#define FE_IFACE_H

class Parser;

#include <QMessageBox>
#include <QTextStream>
#include <QRegExp>
#include "../Graph/graph_iface.h"
#include "../QtGUI/gui_iface.h"
#include "symtab.h"
#include "parser.h"
#include "fe_gcc.h"
#include "fe_test.h"

#endif /* #ifndef FE_IFACE_H */