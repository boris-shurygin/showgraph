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
TextView::TextView(): hl( NULL)
{
	setOpenLinks( false);
}

/**
 * Highlight text
 */
void TextView::highlighText()
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
