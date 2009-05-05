/**
 * File: gui_impl.h - Implementational header for QtGUI project
 * GUI for ShowGraph tool. Adaptaion from XML example of QT toolkit.
 * Copyright (C) 2009  Boris Shurygin
 */
#include <QtGui>

#include "main_window.h"

MainWindow::MainWindow()
{
    graph_view = new GraphView();
    setCentralWidget( graph_view);

    createActions();
    createMenus();

    statusBar()->showMessage(tr("Ready"));

    setWindowTitle(tr("ShowGraph"));
    resize(480, 320);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open graph File"),
                                         QDir::currentPath(),
                                         tr("Graph Files ( *.xml)"));
    if (fileName.isEmpty())
        return;
    delete graph_view;

    graph_view = new GraphView();
    setCentralWidget( graph_view);
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Graph Description"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    file.close();
    graph_view->readFromXML( fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::newGraph()
{
    delete graph_view;

    graph_view = new GraphView();
    setCentralWidget( graph_view);
    statusBar()->showMessage(tr("Created new"), 2000);
}

void MainWindow::runLayout()
{
    graph_view->rankNodes();
    statusBar()->showMessage(tr("Layout done"), 2000);
}

void MainWindow::saveAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save Graph File"),
                                         QDir::currentPath(),
                                         tr("Graph Files ( *.xml)"));
    if (fileName.isEmpty())
        return;
    graph_view->writeToXML( fileName);
    /*QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    if ( graph_view->write(&file))
        statusBar()->showMessage(tr("File saved"), 2000);*/
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Showgraph"),
                      tr("The <b>ShowGraph</b> implements a simple graph editor currently."));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    saveAsAct->setShortcut(tr("Ctrl+S"));
    connect( saveAsAct, SIGNAL(triggered()), this, SLOT( saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect( exitAct, SIGNAL(triggered()), this, SLOT( close()));

    aboutAct = new QAction(tr("&About"), this);
    connect( aboutAct, SIGNAL(triggered()), this, SLOT( about()));

    newGraphAct = new QAction(tr("&New"), this);
    newGraphAct->setShortcut(tr("Ctrl+N"));
    connect( newGraphAct, SIGNAL(triggered()), this, SLOT( newGraph()));

    layoutRunAct = new QAction(tr("&Run"), this);
    layoutRunAct->setShortcut(tr("F5"));
    connect( layoutRunAct, SIGNAL(triggered()), this, SLOT( runLayout()));


}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File"));
    fileMenu->addAction( newGraphAct);
    fileMenu->addAction( openAct);
    fileMenu->addAction( saveAsAct);
    fileMenu->addAction( exitAct);

    menuBar()->addSeparator();

    layoutMenu = menuBar()->addMenu( tr( "Layout"));
    layoutMenu->addAction( layoutRunAct);

    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
