/**
 * @file: main_window.h 
 * Implementational header for MainWindow class in QtGUI project
 */
/*
 * GUI for ShowGraph tool. Adaptaion from XML example of QT toolkit.
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
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "gui_impl.h"
#include <QMainWindow>

/** Magnifying factor for getting more detailed images */
const int IMAGE_EXPORT_SCALE_FACTOR = 2;

/** Adjust value for image's bounding rectangle on scene rendering */
const qreal IMAGE_RECT_ADJUST = 10;

/**
 * Search modes
 */
enum FindMode
{
	/** Node searching */
	FIND_MODE_NODE,
	/** Default */
	FIND_MODE_DEFAULT = FIND_MODE_NODE,
	/** IR dump text searching  */
	FIND_MODE_TEXT,
	/** Number of searching modes */
	FIND_MODES_NUM
};

/** Find widget */
class FindWidget : public QWidget
{
    Q_OBJECT

public: 
    /** Constructor */
	FindWidget(QWidget *parent = 0);
    /** Destructor */
	~FindWidget();
	/** Get current searching mode */
	inline FindMode mode() const;
signals:
    /** Signals that 'next'( or '->') button is pressed */
    void findNext();
    /** Signals that 'prev'( or '<-') button is pressed */
    void findPrevious();

private slots:
    /** Update the look of widget and hide/show buttons according to mode */
    void updateButtons();
    /** React to mode change */
	void modeSet();

private:
    QLineEdit *editFind;
    QCheckBox *checkCase;
    QLabel *labelWrapped;
    QToolButton *toolNext;
    QToolButton *toolClose;
    QToolButton *toolPrevious;
    QCheckBox *checkWholeWords;
	QComboBox *comboMode;
	friend class MainWindow;
};

/** Get current searching mode */
FindMode FindWidget::mode() const
{
	return ( FindMode)comboMode->itemData( comboMode->currentIndex()).toInt();
}

/**
 * Main window class
 * @ingroup GUI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT;

private:
    QWidget *view;
	FindWidget *findWidget;
	TestParser *parser;
    GraphView* graph_view;
    QProgressBar* progress_bar;
    QList< QDockWidget *> textDocks;
	Conf* conf;
public:
    /** Constructor */
    MainWindow();
    ~MainWindow();
public slots:
    /** Show the open file dialog and open selected file */
    void open();
    /** Create new graph view */
    void newGraph();
    /** Run layout procedure on the current graph view */   
    void runLayout();
    /** Perform actions after layout */
    void layoutDone();
    /** Save graph representation */   
    void saveAs();
    /** Show "about" info */
    void about();
	/** Find node/text next */
	void findNext();
	/** Find node/text prev */
	void findPrev();
    /** Zoom view in */
    void zoomIn();
    /** Zoom view out */
    void zoomOut();
    /** Restore original zoom ratio */
    void zoomOrig();
    /** Show text window for selected node */
    void showNodeText( GNode *node);
    /** Show text window for selected node */
    void exportImage();
    /** Print picture */
    void printContents();
    /** React to anchor click in text view */
	void textClicked( const QUrl & link);
public:
    /** Reimplementation of drag enter event handler */
	void dragEnterEvent( QDragEnterEvent *event);
	/** Reimplementation of drop event handler */
	void dropEvent( QDropEvent *event);
	/** Reimplementation of drag move event handler */
	void dragMoveEvent( QDragMoveEvent *event);
	/** Open specified file */
	void openFile( QString fileName);
private:
    /** Create actions */
    void createActions();
    /** Create menus */
    void createMenus();
    /** Create toolbar */
    void createToolbar();
    /** Discard the current graph view */
    void removeGraphView();
    /** Connect signals and slots to newly created graph view */
    void connectToGraphView( GraphView *view);
    
    bool findText( QString &str, bool forward);
    /** Find text string in view */
    bool findInView( QString &str,
                     TextView* view, 
                     bool forward,
                     QTextDocument::FindFlags flags);

	QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *trackFocusAct;
    QAction *openAct;
    QAction *newGraphAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *layoutRunAct;
    QAction *contextViewAct;
	QAction *findAct;
    QAction *navPrevAct;
    QAction *navNextAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomOrigAct;
    QAction *exportImageAct;
    QAction *printAct;
    QToolBar *tool_bar;
    QToolBar *find_tool_bar;
};

#endif
