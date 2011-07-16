/**
 * @file: gui_impl.h 
 * Implementational rendering for command-line mode
 */
/*
 * Console version of ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <QRegExp>
#include "../Core/GraphView/graph_view.h"
#include "../QtGUI/gui_iface.h"
#include "../Frontend/fe_iface.h"

/**
 * Object that performs rendering
 */
class Renderer: public QObject
{
public:
    /** Render specified graph to specified image file */
    bool render( QString xmlname, QString outname);
};

#endif /* RENDER_H */