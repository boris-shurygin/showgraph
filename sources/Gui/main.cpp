/**
 * @file: Gui/main.cpp
 * Implementation and entry point for GUI version of ShowGraph
 */
/*
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
#include <stdio.h>
#include <QRegExp>
#include "../Core/GraphView/gview_iface.h"
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
