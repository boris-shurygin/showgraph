/**
 * @file: Gui/main.cpp
 * Implementation and entry point for GUI version of ShowGraph
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include <stdio.h>
#include <QRegExp>
#include "../Utils/utils_iface.h"
#include "../QtGUI/gui_iface.h"
#include "../Frontend/fe_iface.h"
#include <windows.h>

/**
 * The entry point for GUI version of ShowGraph
 */
int main(int argc, char **argv)
{
    Conf conf;
    conf.addOption( new Option( OPT_STRING, "f", "file", "input graph description file name"));
    conf.readArgs( argc, argv);
    
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    
    MainWindow mainWin;
    mainWin.show();
    
    return app.exec();
}