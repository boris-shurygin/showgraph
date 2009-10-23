/**
 * @file: main_window.h 
 * Implementational header for MainWindow class in QtGUI project
 */
/*
 * GUI for ShowGraph tool. Adaptaion from XML example of QT toolkit.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "gui_impl.h"
#include <QMainWindow>

/**
 * Main window class
 * @ingroup GUI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    GraphView* graph_view;
    QDockWidget *dock;
public:
    /** Constructor */
    MainWindow();

public slots:
    /** Show the open file dialog and open selected file */
    void open();
    /** Create new graph view */
    void newGraph();
    /** Run layout procedure on the current graph view */   
    void runLayout();
    /** Save graph representation */   
    void saveAs();
    /** Show "about" info */
    void about();
    /** Zoom view in */
    void zoomIn();
    /** Zoom view out */
    void zoomOut();
    /** Restore original zoom ratio */
    void zoomOrig();
    /** Show text window for selected node */
    void showNodeText( GNode *node);
    /** Export picture */
    void exportImage();
    /** Print picture */
    void printContents();

private:
    /** Create actions */
    void createActions();
    /** Create menus */
    void createMenus();
    /** Discard the current graph view */
    void removeGraphView();
    /** Connect signals and slots to newly created graph view */
    void connectToGraphView( GraphView *view);

	QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *newGraphAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *layoutRunAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomOrigAct;
    QAction *exportImageAct;
    QAction *printAct;
};

#endif
