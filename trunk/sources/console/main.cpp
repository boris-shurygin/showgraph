/**
 * @file: console/main.cpp 
 * Implementation of console version of showgraph
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "render.h"

/**
 * In debug mode project is linked with Qt's dll-oriented build.
 * Plugins are supported without any additional actions
 */
#ifndef _DEBUG
  #include <QtPlugin>
  //Q_IMPORT_PLUGIN(qico);
  //Q_IMPORT_PLUGIN(qgif);
  //Q_IMPORT_PLUGIN(qjpeg);
  //Q_IMPORT_PLUGIN(qmng);
  //Q_IMPORT_PLUGIN(qtiff);
#endif

/**
 * The entry point for console version of ShowGraph
 */
static int doAll( int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    Conf conf;
    conf.addOption( new Option( OPT_STRING, "f", "file", "input graph description file name"));
    conf.addOption( new Option( OPT_STRING, "o", "output", "output image file name"));
    conf.readArgs( argc, argv);
    
    Option *fopt = conf.longOption("file");
    Option *out_opt = conf.longOption("output");
    
    assertd( isNotNullP( fopt));
    assertd( isNotNullP( out_opt));
    if ( fopt->isDefined())
    {
        QString xmlname = fopt->string();
        QString outname("image.png");
        Renderer r;
        if ( out_opt->isDefined())
        {
            outname = out_opt->string();
        }
        r.render( xmlname, outname);
    } else
    {
        conf.printOpts(); // Print options to console
    }
    //return app.exec();
    return 0;
}

/**
 * Main routine
 */
int main( int argc, char **argv)
{
    MemMgr::init();
    
    int res = doAll( argc, argv);
           
    MemMgr::deinit();
    
    return res;
}
