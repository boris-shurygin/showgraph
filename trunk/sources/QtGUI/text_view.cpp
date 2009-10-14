/**
 * @file: text_view.cpp
 * TextView class implementation
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

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