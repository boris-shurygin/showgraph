/**
 * File: main_window.h - Implementational header for MainWindow class in QtGUI project
 * GUI for ShowGraph tool. Adaptaion from XML example of QT toolkit.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "gui_impl.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    GraphView* graph_view;
    QDockWidget *dock;
public:
    MainWindow();

public slots:
    void open();
    void newGraph();
    void runLayout();
    void saveAs();
    void about();
    void zoomIn();
    void zoomOut();
    void zoomOrig();
    void showNodeText( GNode *node);

private:
    void createActions();
    void createMenus();
    void removeGraphView();
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
};

#endif
