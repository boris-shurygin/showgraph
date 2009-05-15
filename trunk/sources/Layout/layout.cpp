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
        foreach ( AuxNode* node, nodes)
        {
            node->orig()->setPos( x, node->orig()->y());
            x = x + node->orig()->boundingRect().width() + 20;
        }
    } 
}