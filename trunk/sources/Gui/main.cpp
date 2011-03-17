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

/**
 * In debug mode project is linked with Qt's dll-oriented build.
 * Plugins are supported without any additional actions
 */
#ifdef _MSC_VER
#  ifndef _DEBUG
#    include <QtPlugin>
  Q_IMPORT_PLUGIN(qico);
  //Q_IMPORT_PLUGIN(qgif);
  //Q_IMPORT_PLUGIN(qjpeg);
  //Q_IMPORT_PLUGIN(qmng);
  //Q_IMPORT_PLUGIN(qtiff);
#  endif
#endif

/**
 * all the actions
 */
static int doAll( int argc, char **argv)
{
    Conf conf;
    conf.addOption( new Option( OPT_STRING, "f", "file", "input graph description file name"));
    conf.readArgs( argc, argv);
    
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    
    MainWindow mainWin;
    mainWin.show();

    Option *fopt = conf.longOption("file");
    assertd( isNotNullP( fopt));

    if ( fopt->isDefined())
    {
        QString filename = fopt->string();
        mainWin.openFile( filename);
    }
    
    return app.exec();
}

/**
 * The entry point for GUI version of ShowGraph
 */
int main(int argc, char **argv)
{
    MemMgr::init();
    
    int res = doAll( argc, argv);
           
    MemMgr::deinit();
    
    return res;
}
