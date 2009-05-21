/**
 * File: layout.cpp - Layout algorithms implementation file
 * Layout library, 2d graph placement of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "layout_iface.h"

bool compareOrders( AuxNode* node1,
                    AuxNode* node2)
{
    return ( node1->order() < node2->order());
}
void Level::sortNodesByOrder()
{
    qSort( node_list.begin(), node_list.end(), compareOrders);
}

void AuxGraph::orderNodesByDFS()
{
    /** Structure used for dfs traversal */
    struct DfsStepInfo
    {
        AuxNode *node; // Node in consideration
        AuxEdge *edge; // Next edge

        /* Constructor */
        DfsStepInfo( AuxNode *n)
        {
            node = n;
            edge = n->firstSucc();
        }
    };

    Marker m = newMarker(); // Marker for visiting nodes
    QStack< DfsStepInfo *> stack;
    GraphNum num = 0;
    
    /* Fill stack with nodes that have no predecessors */
    for ( AuxNode *n = firstNode();
          isNotNullP( n);
          n = n->nextNode())
    {
        if ( isNullP( n->firstPred()) && !n->isMarked( m))
        {
            n->setOrder( num++);
            stack.push( new DfsStepInfo( n));

            /* Walk graph with marker and perform classification */
            while( !stack.isEmpty())
            {
                DfsStepInfo *info = stack.top();
                AuxNode *node = info->node;
                AuxEdge *edge = info->edge;
                
                if ( isNotNullP( edge)) // Add successor to stack
                {
                    AuxNode* succ_node = edge->succ();
                    info->edge = edge->nextSucc();
                    
                    if ( succ_node->mark( m))
                    {
                        stack.push( new DfsStepInfo( succ_node));
                        succ_node->setOrder( num++);
                    }
                } else // We're done with this node
                {
                    delete info;
                    stack.pop();
                }
            }
        }
    }
    freeMarker( m);
}
void AuxGraph::reduceCrossings()
{
    /** Perform numeration and sort nodes to avoid tree edges crossings */
    orderNodesByDFS();

    for ( int i = 0; i < levels.size(); i++)
    {
        Level* level = levels[ i];
        level->sortNodesByOrder();
    }
}

void AuxGraph::arrangeHorizontally()
{
    for ( int i = 0; i < levels.size(); i++)
    {
        Level* level = levels[ i];
        QList< AuxNode*> nodes = level->nodes();
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