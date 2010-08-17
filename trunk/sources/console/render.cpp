/**
 * @file: render.cpp
 * Implementation of graph rendering to file
 */
/*
 * Console version of ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "render.h"

/**
 * Render picture for graph described in XML
 */
bool Renderer::render( QString xmlname, QString outname)
{
    QTextStream sout( stdout);
    QTextStream serr( stdout);
    
    QFile file( xmlname);
    
    if ( !file.open(QFile::ReadOnly | QFile::Text))
    {
        serr << tr("Cannot read file %1:\n%2.")
                .arg( xmlname)
                .arg( file.errorString());
        return false;
    }
    /** Create graph instance */
    GraphView* graph_view = new GraphView();
    graph_view->setGraph( new GGraph( graph_view));

    /** Read graph from XML */
    graph_view->graph()->readFromXML( xmlname);
 
    /**
     * Perform layout in single thread. 
     * Multi thread wouldn't work since we do not run QApplication::exec() and there is not event loop
     */
    graph_view->graph()->doLayoutSingle();

    /** Get scene rectangle */
    QRectF scene_rect( graph_view->scene()->itemsBoundingRect()
                       .adjusted( -IMAGE_RECT_ADJUST, -IMAGE_RECT_ADJUST,
                                   IMAGE_RECT_ADJUST, IMAGE_RECT_ADJUST));

    /** Render to image */
	QImage image( scene_rect.width() * IMAGE_EXPORT_SCALE_FACTOR,
		          scene_rect.height() * IMAGE_EXPORT_SCALE_FACTOR,
				  QImage::Format_RGB32);
    image.fill( graph_view->palette().base().color().rgb());
    QPainter pp( &image);
	pp.setRenderHints( graph_view->renderHints());
    graph_view->scene()->render( &pp, image.rect(), scene_rect);
    QImageWriter writer( outname);
    
    /** Write image to file */
    if ( writer.canWrite() && writer.write( image))
    {
        sout << tr("Rendered %1 to %2\n")
                .arg( xmlname)
                .arg( outname);
       
    } else
    {
        serr << tr("Cannot write file %1:\n%2\n")
                .arg( xmlname)
                .arg( writer.errorString());
        return false;
    }
    return true;
};
