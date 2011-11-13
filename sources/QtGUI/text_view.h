/**
 * @file: text_view.h 
 * TextView ( IR dump browser) class definition
 */
/* 
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

/**
 * Class used to highlight the dump and create anchors
 */
class DumpHighlighter: public QSyntaxHighlighter
{

public:
    /**
     * Constructor from text document
     */
    DumpHighlighter( QTextDocument *doc): QSyntaxHighlighter( doc){};

    /**
     * Highlight given text
     */
    void highlightBlock(const QString &text);
};


/**
 * IR dump browser class
 */
class TextView: public QTextBrowser
{
    //Q_OBJECT; /** For MOC */

    /** Highlighter used to format the text */
    DumpHighlighter * hl;

    CFNode *node;
public:
    /**
     * Constructor
     */
    TextView( CFNode *n);

    /**
     * Destructor
     */
    virtual ~TextView();

    /**
     * Open document in browser
     */
    void openFile( QString fileName);

    /**
     * Highlight text
     */
    void highlightText();
    
    QString nodeIdClicked( QPoint pos);
    void mouseReleaseEvent( QMouseEvent * mouseEvent);
    void mousePressEvent( QMouseEvent * mouseEvent);
    void contextMenuEvent(QContextMenuEvent *event);
};


#endif /* TEXT_VIEW_H */
