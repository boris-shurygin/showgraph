/**
 * @file: text_view.cpp
 * TextView class implementation
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
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
TextView::TextView( GNode *n): node( n), hl( NULL)
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

bool TextView::nodeIdClicked( QPoint pos, int *id_p)
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

        if ( !href.isEmpty())
        {
            bool success = false;
            int id = href.toInt( &success);
            if ( success)
            {
                if ( isNotNullP( id_p))
                {
                    *id_p = id;
                }
                return true;
            }
        }
    }
    return false;
}

void TextView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu;
      
    if ( nodeIdClicked( event->pos(), NULL))
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
        int id = -1;
        if ( nodeIdClicked( ev->pos(), &id))
        {
            GNode *new_node = node->graph()->view()->findNodeById( id);
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
                    highlightText();
                    node = new_node;
                    dock->setWindowTitle( new_node->item()->toPlainText());
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
