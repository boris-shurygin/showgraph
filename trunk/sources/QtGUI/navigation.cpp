/**
 * @file: navigation.cpp 
 * Implemetation of navigation helper classes
 * @ingroup GUIGraph
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

#include "gview_impl.h"

/** Constructor */
NodeNav::NodeNav( GNode *curr_node, NavSector nav_sector):
    node_priv( curr_node),
    sector_priv( nav_sector)
{

}
    
/** Get edge to the bottom of current node */
GEdge *
NodeNav::edgeDown() const
{
    return NULL;
}

/** Get edge to the left of given edge */
GEdge *
NodeNav::edgeLeft( GEdge * edge) const
{
    /* Applicable only for top and bottom sectors */
    if ( sector() == LEFT_SECTOR
         || sector() == RIGHT_SECTOR
         || sector() == UNDEF_SECTOR)
    {
        return NULL;
    }
    
    GNode *n = otherEnd( edge);
    /* item corresponding to other (than node_priv) node */
    if ( isNullP( n))
        return NULL;

    NodeItem *item = n->item();
    
    // in current node's coordinates
    QPointF edge_point = node()->item()->mapFromItem( item, item->boundingRect().center());

    GEdge *res = NULL;
    qreal min_delta_x = 0;
    GEdge *e;
    foreachPred( e, node())
    {
        if ( isEdgeInSector( e))
        {
            NodeItem *p_item = e->pred()->item();
            QPointF point = node()->item()->mapFromItem( p_item, p_item->boundingRect().center());
            if ( point.x() <= edge_point.x()) // 'left' property check
            {
                if ( isNullP( res) 
                     || edge_point.x() - point.x() < min_delta_x) // for selection of closest edge
                {
                    res = e;
                    min_delta_x = edge_point.x() - point.x();
                }
            }
        }
    }
    foreachSucc( e, node())
    {
        if ( isEdgeInSector( e))
        {
            NodeItem *p_item = e->succ()->item();
            QPointF point = node()->item()->mapFromItem( p_item, p_item->boundingRect().center());
            if ( point.x() <= edge_point.x()) // 'left' property check
            {
                if ( isNullP( res) 
                     || edge_point.x() - point.x() < min_delta_x) // for selection of closest edge
                {
                    res = e;
                    min_delta_x = edge_point.x() - point.x();
                }
            }
        }
    }
    return res;
}

/* Other edge's node */
GNode *
NodeNav::otherEnd( GEdge *edge) const
{
    if ( areEqP( node_priv, edge->pred()))
    {
        return edge->succ();
    } else if ( areEqP( node_priv, edge->succ()))
    {
        return edge->pred();
    }
    return NULL;
}

/** Check that given edge is in current sector */
bool NodeNav::isEdgeInSector( GEdge * edge) const
{
    GNode *n = otherEnd( edge);
    /* item corresponding to other (than node_priv) node */
    if ( isNullP( n) || sector() == UNDEF_SECTOR)
        return false;

    NodeItem *item = n->item();
    
    // Both points in current node's coordinates
    QPointF p_center = node_priv->item()->boundingRect().center();
    QPointF s_center = node_priv->item()->mapFromItem( item, item->boundingRect().center());

    qreal angle = QLineF( p_center, s_center).angle();
    qreal max_angle = sectorMaxAngle();
    qreal min_angle = sectorMinAngle();

    if ( angle <= max_angle && angle >= min_angle)
        return true;
    return false;
}

/** Compute max angle for sector */
qreal NodeNav::sectorMaxAngle() const
{
    QLineF line;
    QRectF rect = node_priv->item()->boundingRect();
    QPointF center = rect.center();
    
    switch ( sector_priv)
    {
        case TOP_SECTOR:
            line = QLineF( center, rect.topLeft());
            break;
        case BOTTOM_SECTOR:
            line = QLineF( center, rect.bottomRight());
            break;
        case LEFT_SECTOR:
            line = QLineF( center, rect.bottomLeft());
            break;
        case RIGHT_SECTOR:
            line = QLineF( center, rect.topRight());
            break;
        case UNDEF_SECTOR:
        default:
            return 0;
    }
    return line.angle();
}

/** Compute min angle for sector */
qreal NodeNav::sectorMinAngle() const
{
    QLineF line;
    QRectF rect = node_priv->item()->boundingRect();
    QPointF center = rect.center();
    
    switch ( sector_priv)
    {
        case TOP_SECTOR:
            line = QLineF( center, rect.topRight());
            break;
        case BOTTOM_SECTOR:
            line = QLineF( center, rect.bottomLeft());
            break;
        case LEFT_SECTOR:
            line = QLineF( center, rect.topLeft());
            break;
        case RIGHT_SECTOR:
            line = QLineF( center, rect.bottomRight());
            break;
        case UNDEF_SECTOR:
        default:
            return 0;
    }
    return line.angle();
}