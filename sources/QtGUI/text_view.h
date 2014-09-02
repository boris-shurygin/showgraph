/**
 * @file: text_view.h 
 * TextView ( IR dump browser) class definition
 */
/* 
 * GUI for ShowGraph tool.
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
