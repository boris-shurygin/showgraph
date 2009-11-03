/**
 * File: UnitTest/main.cpp - Implementation and entry point for unit testing of ShowGraph
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"

int main(int argc, char **argv)
{
#ifdef _DEBUG
	/** Test memory management */
    if ( !uTestMem())
		return -1;

    /** Test configuration classes functionality */
    if ( !uTestConf())
    	return -1;

	/** Test graph package */
    if ( !uTestGraph())
        return -1;

    /** Test frontend */
    if ( !uTestFE())
        return -1;
    
    /** Test graph package */
    if ( uTestGUI(argc, argv) != 0)
        return -1;
    
    return 0;
#else
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    
    MainWindow mainWin;
    mainWin.show();
    
    return app.exec();
#endif
}