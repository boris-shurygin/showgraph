/**
 * @file: render.cpp
 * Implementation of graph rendering to file
 */
/*
 * Console version of ShowGraph tool.
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
    graph_view->setGraph( new GGraph( graph_view, true));

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
