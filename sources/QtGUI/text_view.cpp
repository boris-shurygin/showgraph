/**
 * @file: text_view.cpp
 * TextView class implementation
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
#include "gui_impl.h"

/**
 * Highlight given text
 */
void DumpHighlighter::highlightBlock(const QString &text)
{
     QTextCharFormat myClassFormat;
     myClassFormat.setFontWeight(QFont::Bold);
     myClassFormat.setForeground(Qt::darkMagenta);
	 myClassFormat.setAnchor( true);

	 QRegExp exp("Node (\\d+)");

	 int index = text.indexOf( exp);
     while (index >= 0)
	 {
         int length = exp.matchedLength();
         
		 myClassFormat.setAnchorHref( exp.cap( 1));
		 setFormat(index, length, myClassFormat);
         
		 index = text.indexOf( exp, index + length);
     }
}

/**
 * TextView Constructor
 */
TextView::TextView( CFNode *n): node( n), hl( NULL)
{
	setOpenLinks( false);
}

/**
 * Highlight text
 */
void TextView::highlightText()
{
	 //hl = new DumpHighlighter( document());
	 QTextCursor cursor( document());
	 QString text = document()->toPlainText();
	 QRegExp exp("Node (\\d+)");
	 int index = text.indexOf( exp);
     while (index >= 0)
	 {
         int length = exp.matchedLength();
         
		 QTextCharFormat link_fmt;
		 //link_fmt.setFontWeight(QFont::Bold);
		 link_fmt.setForeground(Qt::blue);
		 link_fmt.setFontUnderline( true);
		 link_fmt.setAnchor( true);
		 link_fmt.setAnchorHref( exp.cap( 1));
		 cursor.setPosition( index);
		 cursor.setPosition( index + length, QTextCursor::KeepAnchor);
		 cursor.removeSelectedText();
		 cursor.insertText( QString( "Node %1").arg( exp.cap( 1)), link_fmt);
		 index = text.indexOf( exp, index + length);
     }

     QRegExp edge_rx("CF EDGE (\\d+) \\[(\\d+)->(\\d+)\\]");
	 index = text.indexOf( edge_rx);
     while (index >= 0)
	 {
         int length = edge_rx.matchedLength();
         
		 QTextCharFormat plain;
		 QTextCharFormat link_fmt;
		 //link_fmt.setFontWeight(QFont::Bold);
		 link_fmt.setForeground(Qt::blue);
		 link_fmt.setFontUnderline( true);
		 link_fmt.setAnchor( true);
		 link_fmt.setAnchorHref( edge_rx.cap( 2));
		 cursor.setPosition( index);
		 cursor.setPosition( index + length, QTextCursor::KeepAnchor);
		 cursor.removeSelectedText();

		 cursor.insertText( QString( "CF EDGE %1 [").arg( edge_rx.cap( 1)), plain);
		 cursor.insertText( edge_rx.cap( 2), link_fmt);
		 cursor.insertText( QString( "->"), plain);
		 link_fmt.setAnchorHref( edge_rx.cap( 3));
		 cursor.insertText( edge_rx.cap( 3), link_fmt);
		 cursor.insertText( QString( "]"), plain);
		 
		 index = text.indexOf( edge_rx, index + length);
     }
}

void
TextView::openFile( QString fileName)
{
     if ( !QFile::exists( fileName))
         return;

     QFile file( fileName);
     if ( !file.open(QFile::ReadOnly))
         return;

     QByteArray data = file.readAll();
     QTextCodec *codec = Qt::codecForHtml(data);
     QString str = codec->toUnicode(data);
     
     if ( Qt::mightBeRichText(str))
     {
         setHtml(str);
     } else
     {
         str = QString::fromLocal8Bit(data);
         setPlainText(str);
     }
}

QString TextView::nodeIdClicked( QPoint pos)
{
    QString str = anchorAt( pos);
        
    if ( !str.isEmpty())
    {
        QTextCursor cursor = cursorForPosition ( pos);
        QString href = cursor.charFormat().anchorHref();
        
        if ( href.isEmpty())
        {
            cursor.movePosition( QTextCursor::NextCharacter);
            href = cursor.charFormat().anchorHref();
        }
        return href;
    }
    return str;
}

void TextView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu;
    QString id_clicked = nodeIdClicked( event->pos());
    if ( !id_clicked.isEmpty())
    {
        menu = new QMenu( this);
        menu->addAction(tr("My Menu Item"));
    } else
    {
        menu = createStandardContextMenu();
    }
    menu->exec( event->globalPos());
    delete menu;
}

void TextView::mouseReleaseEvent( QMouseEvent * ev)
{
    if ( ev->button() & Qt::LeftButton)
    {
        /** Change node */
        QString href = nodeIdClicked( ev->pos());
        
        if ( !href.isEmpty())
        {
            CFNode *new_node;
            bool success = false;
            int id = href.toInt( &success);
            if ( success)
            {
                new_node = static_cast< CFNode *>( node->graph()->view()->findNodeById( id));
            } else
            {
                new_node = static_cast< CFNode *>( node->graph()->view()->findNodeByLabel( href));
            }
            
            if ( isNotNullP( new_node)) 
            {
                if( !new_node->isTextShown())
                {
                    QDockWidget *dock = node->item()->textDock();
                    node->setTextShown( false);
                    node->item()->setTextDock( NULL);
                    new_node->item()->setTextDock( dock);
                    new_node->setTextShown();
                    document()->clear();
                    setPlainText( new_node->doc()->toPlainText());
                    new_node->graph()->parser()->highlightText( document());
                    node = new_node;
                    dock->setWindowTitle( QString("Node %1").arg( node->irId()));
                } else
                {
	                new_node->item()->textDock()->show();
                }
            }        
        }
    }

    QTextBrowser::mouseReleaseEvent( ev);
}

void TextView::mousePressEvent( QMouseEvent * mouseEvent)
{
	QTextBrowser::mousePressEvent( mouseEvent);
}

/**
 * TextView destructor
 */
TextView::~TextView()
{
	delete hl;
}
