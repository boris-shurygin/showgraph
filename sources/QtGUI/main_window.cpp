/**
 * @file: main_window.cpp 
 * Implementation of MainWindow class
 */
/*
 * GUI for ShowGraph tool. Adaptaion from XML example of QT toolkit.
 * Copyright (C) 2009  Boris Shurygin
 */
#include <QtGui>

#include "main_window.h"

MainWindow::MainWindow()
{
    graph_view = new GraphView();
    setCentralWidget( graph_view);

    dock = new QDockWidget( tr("Text"), this);
    
    createActions();
    createMenus();

    statusBar()->showMessage(tr("Ready"));

    setWindowTitle(tr("ShowGraph"));
    resize(480, 320);
        
    TextView* text_view = new TextView();
    dock->setWidget( text_view);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->hide();
}

void MainWindow::showNodeText( GNode *node)
{
    TextView* text_view = ( TextView* )dock->widget();
    if ( isNotNullP( node->doc()))
        text_view->setPlainText( node->doc()->toPlainText());
}

void MainWindow::removeGraphView()
{
    /** Delete old graph and text views */
    delete graph_view;

    /*if ( isNotNullP( dock->widget()))
        delete dock->widget();*/
    dock->hide();
}

void MainWindow::connectToGraphView( GraphView *view)
{
    graph_view = view;
    
    /** Connect signals */
    connect( view, SIGNAL( nodeClicked( GNode*)), this, SLOT( showNodeText( GNode*)));

    /** Place graph view in window */
    setCentralWidget( graph_view);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open graph File"),
                                         QDir::currentPath(),
                                         tr("Dump/Text Files ( *.txt);;Graph XML Files ( *.xml);;All Files ( *.*)"));
    if ( fileName.isEmpty())
        return;

    removeGraphView();

    QRegExp rx("\\.xml$");
    
    bool do_layout = false;

    /** Not a graph description - run parser */
    if ( rx.indexIn( fileName) == -1 )
    {
        //TextView* text_view = dock->widget();
        //text_view->openFile( fileName);
        TestParser parser( fileName);
        parser.mainLoop();
        connectToGraphView( parser.graphView());
        do_layout = true;
        dock->show();
    } else
    {
        QFile file(fileName);
        if ( !file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Graph Description"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        file.close();
        connectToGraphView( new GraphView());
        graph_view->graph()->readFromXML( fileName);
    }
    
    /** Run layout automatically */
    if ( do_layout)
        graph_view->graph()->doLayout();
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::zoomIn()
{
    graph_view->zoomIn();
}

void MainWindow::zoomOut()
{
	graph_view->zoomOut();
}

void MainWindow::zoomOrig()
{
	graph_view->zoomOrig();
}

void MainWindow::newGraph()
{
    delete graph_view;

    graph_view = new GraphView();
    setCentralWidget( graph_view);
    statusBar()->showMessage(tr("Created new"), 2000);
    dock->hide();
}

void MainWindow::runLayout()
{
    graph_view->graph()->doLayout();
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
    
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    file.close();
    graph_view->graph()->writeToXML( fileName);

    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::about()
{
   QMessageBox::about( this,
                       tr("About Showgraph"),
#ifdef _DEBUG
       tr("The <b>ShowGraph</b> implements a simple graph editor currently. <b>DEBUG VERSION</b>")
#else
       tr("The <b>ShowGraph</b> implements a simple graph editor currently.")
#endif
                        );
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

    zoomInAct = new QAction(tr("&Zoom In"), this);
    zoomInAct->setShortcut(Qt::Key_Plus);
    connect( zoomInAct, SIGNAL(triggered()), this, SLOT( zoomIn()));

    zoomOutAct = new QAction(tr("&Zoom Out"), this);
    zoomOutAct->setShortcut(Qt::Key_Minus);
    connect( zoomOutAct, SIGNAL(triggered()), this, SLOT( zoomOut()));

    zoomOrigAct = new QAction(tr("&Original"), this);
    zoomOrigAct->setShortcut(tr("Ctrl+A"));
    connect( zoomOrigAct, SIGNAL(triggered()), this, SLOT( zoomOrig()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File"));
    fileMenu->addAction( newGraphAct);
    fileMenu->addAction( openAct);
    fileMenu->addAction( saveAsAct);
    fileMenu->addAction( exitAct);

    menuBar()->addSeparator();

    viewMenu = menuBar()->addMenu( tr( "&View"));
    viewMenu->addAction( layoutRunAct);
    viewMenu->addSeparator();
    viewMenu->addAction( zoomInAct);
    viewMenu->addAction( zoomOutAct);
    viewMenu->addAction( zoomOrigAct);
    viewMenu->addSeparator();
    viewMenu->addAction( dock->toggleViewAction());
    
    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
