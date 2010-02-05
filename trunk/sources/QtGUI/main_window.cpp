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
    QFrame *central = new QFrame( this);
	central->setMinimumSize( 200, 200);
	dock_find = new QDockWidget( tr("Find"), this);
    QIcon icon( "images/logo.ico");
    setWindowIcon(icon);
    createActions();
    createMenus();

    statusBar()->showMessage(tr("Ready"));

    setWindowTitle(tr("ShowGraph"));
    resize(480, 320);

    addDockWidget( Qt::TopDockWidgetArea, dock_find);
    dock_find->hide();

	vboxLayout = new QVBoxLayout( central);
	vboxLayout->setDirection( QBoxLayout::BottomToTop);
#ifdef Q_OS_MAC
    system = QLatin1String("mac");
#else
    vboxLayout->setMargin(0);
#endif
    view = new QWidget( central);
    
    //findBar = new QWidget( central);
    findWidget = new FindWidget( this);
    //findBar->setMinimumHeight(findWidget->minimumSizeHint().height());
    findWidget->move(0, 0);
    //vboxLayout->addWidget(findBar);
    //findBar->hide();
    findWidget->editFind->installEventFilter( central);
    dock_find->setWidget( findWidget);
    
	//connect(findWidget->toolClose, SIGNAL(clicked()), findBar, SLOT(hide()));
	connect(findWidget->toolNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(findWidget->toolPrevious, SIGNAL(clicked()), this, SLOT(findPrev()));
    connect(findWidget->editFind, SIGNAL(returnPressed()), this, SLOT(findNext()));
   
	setCentralWidget( central);
	
    graph_view = new GraphView();
    graph_view->setGraph( new CFG( graph_view));
    connectToGraphView( graph_view);
    setAcceptDrops( true);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if ( event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();

	if ( mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		if ( urlList.size() == 1)
		{
			QString url = urlList.at(0).toLocalFile();
			openFile( url);
		}
    }

	event->acceptProposedAction();
}
 
void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void MainWindow::printContents()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog( &printer, this);
    dialog->setWindowTitle( tr( "Print Graph"));
    if ( dialog->exec() != QDialog::Accepted)
        return;
    QPainter painter( &printer);
    graph_view->render(&painter);
}

void MainWindow::exportImage()
{
    QString fileName =
        QFileDialog::getSaveFileName( this, tr("Export Image"),
                                      QDir::currentPath(),
                                      tr("Windows Bitmap ( *.bmp);;").
                                      append( tr("Joint Photographic Experts Group (*.jpg);;")).
                                      append( tr("Joint Photographic Experts Group (*.jpeg);;")).
                                      append( tr("Portable Network Graphics( *.png);;")).
                                      append( tr("Tagged Image File Format( *.tiff)")));
    if (fileName.isEmpty())
        return;
    
    QRectF scene_rect( graph_view->scene()->itemsBoundingRect()
                       .adjusted( -IMAGE_RECT_ADJUST, -IMAGE_RECT_ADJUST,
                                   IMAGE_RECT_ADJUST, IMAGE_RECT_ADJUST));
	/** We render */
	QImage image( scene_rect.width() * IMAGE_EXPORT_SCALE_FACTOR,
		          scene_rect.height() * IMAGE_EXPORT_SCALE_FACTOR,
				  QImage::Format_RGB32);
	image.fill( graph_view->palette().base().color().rgb());
    QPainter pp( &image);
	pp.setRenderHints( graph_view->renderHints());
    graph_view->scene()->render( &pp, image.rect(), scene_rect);
    QImageWriter writer( fileName);
    if ( writer.canWrite() && writer.write( image))
    {
        statusBar()->showMessage(tr("Image exported"), 2000);
    } else
    {
        QMessageBox::warning( this, tr("Export Image"),
                              tr("Cannot write file %1:\n%2")
                              .arg( fileName)
                              .arg( writer.errorString()));
        statusBar()->showMessage(tr("Image write failed"), 2000);
    }
}

void MainWindow::textClicked( const QUrl & link)
{
	bool valid = false;
	int node_id = link.toString().toInt( &valid);
	if ( valid)
	{
		graph_view->findNodeById( node_id);
	}
}

void MainWindow::showNodeText( GNode *node)
{
	if ( isNotNullP( node->doc()))
	{
		if ( !node->isTextShown())
		{
			QDockWidget *dock = new QDockWidget( node->item()->toPlainText(), this);
			TextView* text_view = new TextView( node);
			dock->setWidget( text_view);
			addDockWidget(Qt::RightDockWidgetArea, dock);
			text_view->setPlainText( node->doc()->toPlainText());
			node->setTextShown();
			node->item()->setTextDock( dock);
			textDocks.push_back( dock);
			text_view->highlightText();
			connect( text_view, SIGNAL( anchorClicked( const QUrl &)), this, SLOT( textClicked(const QUrl &)));
		} else
		{
			node->item()->textDock()->show();
		}
	}
}

void MainWindow::removeGraphView()
{
	foreach( QDockWidget *tdock, textDocks)
	{
		delete tdock;
	}
	textDocks.clear();

	/** Delete old graph and text views */
    delete graph_view;
}

void MainWindow::connectToGraphView( GraphView *gview)
{
    graph_view = gview;
    
    /** Connect signals */
    connect( gview, SIGNAL( nodeClicked( GNode*)), this, SLOT( showNodeText( GNode*)));
    connect( navPrevAct, SIGNAL( triggered()), gview, SLOT( navPrev()));
    connect( navNextAct, SIGNAL( triggered()), gview, SLOT( navNext()));
    connect( findWidget->editFind, SIGNAL( textChanged( const QString &)),
             gview, SLOT( clearSearch()));
    connect( trackFocusAct, SIGNAL( toggled( bool)),
             gview, SLOT( toggleSmoothFocus( bool)));
    connect( contextViewAct, SIGNAL( toggled( bool)),
             gview, SLOT( toggleViewMode( bool)));
    gview->toggleSmoothFocus( trackFocusAct->isChecked());
    gview->toggleViewMode( contextViewAct->isChecked());
    /** Place graph view in window */
    vboxLayout->addWidget( graph_view);
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName( this, tr("Open graph File"),
                                          QDir::currentPath(),
                                          tr("Dump/Text Files ( *.txt);;Graph XML Files ( *.xml);;All Files ( *.*)"));
    openFile( fileName);
}

void MainWindow::openFile( QString fileName)
{
	if ( fileName.isEmpty())
        return;

    removeGraphView();

    QRegExp rx("\\.xml$");
    
    bool do_layout = false;
    
	QFile file(fileName);
    if ( !file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning( this, tr("Graph Description"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
        return;
    }
    file.close();

    /** Not a graph description - run parser */
    if ( rx.indexIn( fileName) == -1 )
    {
        //TextView* text_view = dock->widget();
        //text_view->openFile( fileName);
        TestParser parser( fileName);
        parser.mainLoop();
        connectToGraphView( parser.graphView());
        do_layout = true;
        //dock->show();
    } else
    {
        graph_view = new GraphView();
        graph_view->setGraph( new CFG( graph_view));
        connectToGraphView( graph_view);
        graph_view->graph()->readFromXML( fileName);
    }
    
    /** Run layout automatically */
    if ( do_layout)
        graph_view->graph()->doLayout();
    statusBar()->showMessage(tr("File %1 loaded").arg( fileName), 2000);
}

void MainWindow::findShow()
{
    findBar->show();
}

bool 
MainWindow::findInView( QString &str,
                        TextView* view,
                        bool forward,
                        QTextDocument::FindFlags flags)
{
    QTextCursor cursor = view->textCursor();
    if ( cursor.atStart() && !forward)
    {
        cursor.movePosition( QTextCursor::End);
    } else if ( cursor.atEnd() && forward)
    {
        cursor.movePosition( QTextCursor::Start);
    }

    if (cursor.hasSelection())
        cursor.setPosition( forward ? cursor.position() : cursor.anchor(),
                            QTextCursor::MoveAnchor);
    view->setTextCursor( cursor);
    QTextCursor newCursor = cursor;
    newCursor = view->document()->find( str, cursor, flags);
    
    if ( newCursor.isNull())
    {
        cursor.movePosition( forward?
                             QTextCursor::End :
                             QTextCursor::Start );
        view->setTextCursor( cursor);
    }
    else{
        view->setTextCursor( newCursor);
    }
    return !newCursor.isNull();
}

bool MainWindow::findText( QString &str, bool forward)
{
    bool res = false;
    QTextCursor cursor;
    QTextDocument *doc = 0;
    QTextDocument::FindFlags flags;

    /** Set text search flags */
    if ( !forward)
        flags |= QTextDocument::FindBackward;
    if ( findWidget->checkCase->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if ( findWidget->checkWholeWords->isChecked())
        flags |= QTextDocument::FindWholeWords;

    GNode * node;
    node = graph_view->searchNode();

    if ( isNullP( node) 
         || !findInView( str,
                         static_cast<TextView *>( node->item()->textDock()->widget()),
                         forward, flags) )
    {
        GNode *prev = node;
        
        node = ( forward) ?
                graph_view->findNextNodeWithText( str, flags) :
                graph_view->findPrevNodeWithText( str, flags); 

        if ( isNotNullP( node))
        {
            showNodeText( node);
            res = true;
            findInView( str,
                        static_cast<TextView *>( node->item()->textDock()->widget()),
                        forward, flags);
        } else
        {
            if ( isNotNullP( prev))
            {
                res = true;
                findWidget->labelWrapped->show();
            } else
            {
                res = false;
            }
        }
    } else
    {
        if ( graph_view->isContext())
        {
            graph_view->graph()->emptySelection();
            graph_view->graph()->selectNode( node);
            graph_view->findContext();
        }
        graph_view->focusOnNode( node, true);
        showNodeText( node);
        res = true;
    }  
    return res;
}

void MainWindow::findNext()
{
	QString findStr = findWidget->editFind->text();
    QPalette p = findWidget->editFind->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::white);
    graph_view->graph()->emptySelection();
                
    findWidget->labelWrapped->hide();
                
    if ( findStr.isEmpty())
    {
        findWidget->editFind->setPalette(p);
        return;
    }

	if ( findWidget->mode() == FIND_MODE_NODE)
	{
		bool goodId = false;
		int id = findStr.toInt( &goodId);
		if ( goodId)
		{
            if ( isNullP( graph_view->findNodeById( id)))
                p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
        } else
		{
		    p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
        }
	} else if ( findWidget->mode() == FIND_MODE_TEXT)
	{
        if ( !findText( findStr, true))
        {
            p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
        }
	}
    findWidget->editFind->setPalette(p);
}

void MainWindow::findPrev()
{
    QString findStr = findWidget->editFind->text();
    QPalette p = findWidget->editFind->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::white);
    graph_view->graph()->emptySelection();
                    
    findWidget->labelWrapped->hide();
    
    if ( findStr.isEmpty())
    {
        findWidget->editFind->setPalette(p);
       return;
    }
    
    assert( findWidget->mode() == FIND_MODE_TEXT);
    
    if ( !findText( findStr, false))
    {
        p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
    }
	findWidget->editFind->setPalette(p);
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
    removeGraphView();

    graph_view = new GraphView();
    graph_view->setGraph( new CFG( graph_view));
    connectToGraphView( graph_view);
    statusBar()->showMessage(tr("Created new"), 2000);
    dock_find->hide();
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
        QMessageBox::warning(this, tr("Showgraph"),
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
       tr("The <b>ShowGraph</b> implements a simple graph editor. <b>DEBUG VERSION</b>")
#else
       tr("The <b>ShowGraph</b> implements a simple graph editor.")
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

    layoutRunAct = new QAction(tr("&Run Layout"), this);
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

    exportImageAct = new QAction(tr("&Export Image..."), this);
    exportImageAct->setShortcut(tr("Ctrl+E"));
    connect( exportImageAct, SIGNAL(triggered()), this, SLOT( exportImage()));

    printAct = new QAction( tr("&Print"), this);
    printAct->setShortcut( tr("Ctrl+P"));
    connect( printAct, SIGNAL( triggered()), this, SLOT( printContents()));

    navPrevAct = new QAction( tr("Navigate &Backward"), this);
    navPrevAct->setShortcut( tr("Ctrl+Left"));
    navNextAct = new QAction( tr("Navigate &Forward"), this);
    navNextAct->setShortcut( tr("Ctrl+Right"));

    trackFocusAct = new QAction( tr("Smooth Centering"), this);
    trackFocusAct->setCheckable( true);
    trackFocusAct->setChecked( false);

    contextViewAct = new QAction( tr("Context View"), this);
    contextViewAct->setCheckable( true);
    contextViewAct->setChecked( false);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File"));
    fileMenu->addAction( newGraphAct);
    fileMenu->addAction( openAct);
    fileMenu->addAction( saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction( exportImageAct);
    fileMenu->addAction( printAct);
    fileMenu->addSeparator();
    fileMenu->addAction( exitAct);
    
    menuBar()->addSeparator();

    viewMenu = menuBar()->addMenu( tr( "&View"));
    viewMenu->addAction( layoutRunAct);
    viewMenu->addAction( contextViewAct);
    viewMenu->addSeparator();
	viewMenu->addAction( zoomInAct);
    viewMenu->addAction( zoomOutAct);
    viewMenu->addAction( zoomOrigAct);
    viewMenu->addAction( trackFocusAct);
    viewMenu->addSeparator();
    
    dock_find->toggleViewAction()->setShortcut(tr("Ctrl+F"));
    viewMenu->addAction( dock_find->toggleViewAction());
    viewMenu->addSeparator();
    viewMenu->addAction( navPrevAct);
    viewMenu->addAction( navNextAct);
    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

FindWidget::FindWidget(QWidget *parent)
    : QWidget(parent)
{
    QString system = QLatin1String("win");
    QHBoxLayout *hboxLayout = new QHBoxLayout(this);
#ifdef Q_OS_MAC
    system = QLatin1String("mac");
#else
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
#endif

    //toolClose = new QToolButton(this);
    //toolClose->setIcon(QIcon(QString::fromUtf8("images/%1/closetab.png").arg(system)));
    //toolClose->setAutoRaise(true);
    //hboxLayout->addWidget(toolClose);
 
    QSpacerItem *spacerItem = new QSpacerItem(5, 5, QSizePolicy::Fixed, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);
    
	comboMode = new QComboBox( this);
	comboMode->addItem( "node", QVariant( FIND_MODE_NODE));
	//comboMode->addItem( "expr", QVariant( FIND_MODE_EXPR)); Off till expression parsing is implemented 
	comboMode->addItem( "text", QVariant( FIND_MODE_TEXT));
	connect( comboMode, SIGNAL( activated(const QString&)),
			 this, SLOT( modeSet()));
	hboxLayout->addWidget( comboMode);

    editFind = new QLineEdit(this);
    editFind->setMinimumSize( QSize(150, 0));
    editFind->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect( editFind, SIGNAL(textChanged(const QString&)),
			 this, SLOT(updateButtons()));
    hboxLayout->addWidget(editFind);

    toolPrevious = new QToolButton(this);
    toolPrevious->setAutoRaise(true);
    //toolPrevious->setText(tr("Previous"));
    toolPrevious->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolPrevious->setIcon(QIcon(QString::fromUtf8("images/%1/previous.png").arg(system)));
    hboxLayout->addWidget(toolPrevious);

    toolNext = new QToolButton(this);
    toolNext->setAutoRaise(true);
    //toolNext->setText(tr("Next"));
    toolNext->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolNext->setIcon(QIcon(QString::fromUtf8("images/%1/next.png").arg(system)));
    hboxLayout->addWidget(toolNext);

    checkCase = new QCheckBox(tr("Case Sensitive"), this);
    //checkCase->hide();
	hboxLayout->addWidget(checkCase);

    checkWholeWords = new QCheckBox(tr("Whole words"), this);
    //checkWholeWords->hide();
	hboxLayout->addWidget(checkWholeWords);
	//toolPrevious->hide();
	
#if defined(USE_WEBKIT)
    checkWholeWords->hide();
#endif

    labelWrapped = new QLabel(this);
    labelWrapped->setMinimumSize(QSize(0, 20));
    labelWrapped->setMaximumSize(QSize(105, 20));
    labelWrapped->setTextFormat(Qt::RichText);
    labelWrapped->setScaledContents(true);
    labelWrapped->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    labelWrapped->setText(tr("<img src=\"images/wrap.png\">&nbsp;Search wrapped"));
    hboxLayout->addWidget(labelWrapped);

    spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);
    setMinimumWidth(minimumSizeHint().width());
    labelWrapped->hide();

    updateButtons();
	modeSet();
}

FindWidget::~FindWidget()
{
}

void FindWidget::updateButtons()
{
    if ( editFind->text().isEmpty())
	{
        toolNext->setEnabled( false);
	    toolPrevious->setEnabled( false);
    } else
	{
        toolPrevious->setEnabled( true);
        toolNext->setEnabled( true);
    }
    
    QPalette p = editFind->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::white);            
    editFind->setPalette(p);
    labelWrapped->hide();
}

void FindWidget::modeSet()
{
	FindMode m = mode();

	switch( m)
	{
	case FIND_MODE_TEXT:
		toolPrevious->show();
		checkCase->show();
		checkWholeWords->show();
		labelWrapped->hide();
        break;
	case FIND_MODE_NODE:
	case FIND_MODE_EXPR:
	default:
		toolPrevious->hide();
		checkCase->hide();
		checkWholeWords->hide();
		labelWrapped->hide();
        break;
	}
}
