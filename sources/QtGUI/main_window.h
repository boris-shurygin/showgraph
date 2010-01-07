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

/** Magnifying factor for getting more detailed images */
const int IMAGE_EXPORT_SCALE_FACTOR = 2;

/** Adjust value for image's bounding rectangle on scene rendering */
const qreal IMAGE_RECT_ADJUST = 10;

/** Find widget */
class FindWidget : public QWidget
{
    Q_OBJECT

public:
    FindWidget(QWidget *parent = 0);
    ~FindWidget();

signals:
    void findNext();
    void findPrevious();

private slots:
    void updateButtons();

private:
    QLineEdit *editFind;
    QCheckBox *checkCase;
    QLabel *labelWrapped;
    QToolButton *toolNext;
    QToolButton *toolClose;
    QToolButton *toolPrevious;
    QCheckBox *checkWholeWords;
	friend class MainWindow;
};


/**
 * Main window class
 * @ingroup GUI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QWidget *view;
	QWidget *findBar;
	FindWidget *findWidget;
	QVBoxLayout *vboxLayout;
    GraphView* graph_view;
    QDockWidget *dock;
    QList< QDockWidget *> textDocks;
	Conf* conf;
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
	/** Show find bar */
	void findShow();
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
    /** Export picture */
    void exportImage();
    /** Print picture */
    void printContents();
	/** Reimplementation of drag enter event handler */
	void dragEnterEvent( QDragEnterEvent *event);
	/** Reimplementation of drop event handler */
	void dropEvent( QDropEvent *event);
	/** Reimplementation of drag move event handler */
	void dragMoveEvent( QDragMoveEvent *event);
	/** Open specified file */
	void openFile( QString fileName);
	/** React to anchor click in text view */
	void textClicked( const QUrl & link);

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
	QAction *findAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomOrigAct;
    QAction *exportImageAct;
    QAction *printAct;
};

#endif
