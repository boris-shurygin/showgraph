/**
 * File: layout.cpp - Layout algorithms implementation file
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "layout_iface.h"

void AuxGraph::reduceCrossings()
{

}

void AuxGraph::arrangeHorizontally()
{
    for ( int i = 0; i < levels.size(); i++)
    {
        Level* level = levels[ i];
        QLinkedList< AuxNode*> nodes = level->nodes();
        qreal x = 0;
        qreal y = 0;
        foreach ( AuxNode* node, nodes)
        {
            if( node->isSimple())
            {
                node->node()->setPos( x, node->node()->y());
                y = node->node()->y();
                x = x + node->node()->boundingRect().width() + 20;
            } else if ( node->isEdgeControl())
            {
                EdgeItem* edge = node->edge();
                EdgeSegment* seg = edge->dstCtrl()->pred();
                EdgeControl* ctrl = seg->addControl( QPointF(x, y));
                ctrl->setFixed();
                edge->adjust();
                x = x + 10;
            }
        }
    } 
}