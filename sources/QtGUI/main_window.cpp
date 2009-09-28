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
    zoom_scale = 0;

    dock = new QDockWidget(tr("Text"), this);
    dock->setWidget( new TextView());
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open graph File"),
                                         QDir::currentPath(),
                                         tr("Graph XML Files ( *.xml);;All Files ( *.*)"));
    if (fileName.isEmpty())
        return;
    delete graph_view;

    graph_view = new GraphView();
    setCentralWidget( graph_view);
    QRegExp rx("\\.xml$");
    
    /** Not a graph description - run parser */
    if ( rx.indexIn( fileName) == -1 )
    {
        delete dock;
        dock = new QDockWidget(tr("Text ").append( fileName), this);
       
        TextView* text_view = new TextView();
        dock->setWidget( text_view);
        text_view->openFile( fileName);
        addDockWidget(Qt::RightDockWidgetArea, dock);

        TestParser parser( fileName);
        parser.convert2XML( fileName.append(".xml"));
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Graph Description"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    file.close();
    graph_view->readFromXML( fileName);
    //graph_view->doLayout();
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::zoomIn()
{
    zoom_scale++;
    updateMatrix();
}

void MainWindow::zoomOut()
{
    zoom_scale--;
    updateMatrix();
}

void MainWindow::zoomOrig()
{
    zoom_scale = 0;
    graph_view->checkDelItems();
    updateMatrix();
}

void MainWindow::updateMatrix()
{
     qreal scale_val = qPow( qreal(2), zoom_scale / qreal(5)); 
     QMatrix matrix;
     matrix.scale(scale_val, scale_val);
   
     graph_view->setMatrix(matrix);
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
    graph_view->doLayout();
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
    graph_view->writeToXML( fileName);

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

    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
