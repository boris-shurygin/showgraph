/**
 * @file: edge_item.cpp 
 * Drawable edge implementation.
 * GUI for ShowGraph tool.
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"
#include <QVector>

//#define SHOW_CONTROL_POINTS
//#define SHOW_BACKEDGES

GEdge::GEdge( GGraph *graph_p, int _id, GNode *_pred, GNode* _succ):
    AuxEdge( (AuxGraph *)graph_p, _id, (AuxNode *)_pred, (AuxNode *)_succ)
{
    item_p = new EdgeItem( this);
}
   
GEdge::~GEdge()
{
    item()->remove();
    graph()->view()->deleteLaterEdgeItem( item());
}

GNode * 
GEdge::node( GraphDir dir) const 
{
    return static_cast< GNode *>(AuxEdge::node( dir));
}

GGraph *
GEdge::graph() const
{
    return static_cast< GGraph *>( AuxEdge::graph());
}

/**
 * Update DOM tree element
 */
void
GEdge::updateElement()
{
    QDomElement e = elem();
    int i = 0;
    QDomElement new_e = graph()->createElement( "edge");
    QDomNode e2 = graph()->documentElement().removeChild( e);
    assert( !e2.isNull());
    graph()->documentElement().appendChild( new_e);
    setElement( new_e);
    e = new_e;

    /* Base class method call to print generic edge properties */
    AuxEdge::updateElement();
}

/**
 * read properties from DOM tree element
 */
void
GEdge::readFromElement( QDomElement e)
{
    AuxEdge::readFromElement( e); // Base class method
    item()->adjust();
}

/**
 * EdgeItem implementaion
 */
/** Constructor */
EdgeItem::EdgeItem( GEdge *e_p): edge_p( e_p)
{
    curr_mode = ModeShow;
    setFlag( ItemIsSelectable);
    //setCacheMode( DeviceCoordinateCache);
    setZValue(1);
}

QVariant
EdgeItem::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if ( change == QGraphicsItem::ItemSelectedChange)
    {
        pred()->item()->update();
        succ()->item()->update();
    }
    return QGraphicsItem::itemChange( change, value);
}    

void
EdgeItem::adjust()
{
    srcP = mapFromItem( pred()->item(), pred()->item()->boundingRect().center());
    dstP = mapFromItem( succ()->item(), succ()->item()->boundingRect().center());
    topLeft = srcP;
    btmRight = dstP;
    QPointF srcCP = srcP;
    QPointF dstCP = dstP;
    if ( pred()->isEdgeControl())
    {

    } else
    {
        QLineF line( srcP, dstP);
        QPolygonF endPolygon = mapFromItem( pred()->item(), pred()->item()->borderRect());
        QPointF p1 = endPolygon.first();
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i);
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                 polyLine.intersect( line, &srcP);
            if (intersectType == QLineF::BoundedIntersection)
                 break;
            p1 = p2;
        }
    }
    if ( succ()->isEdgeControl())
    {
    
    } else
    {
        QLineF line2( srcP, dstP);
        QPolygonF endPolygon = mapFromItem( succ()->item(), succ()->item()->borderRect());
        QPointF p1 = endPolygon.first();;
        QPointF p2;
        QLineF polyLine;
        
        for ( int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i);
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                 polyLine.intersect( line2, &dstP);
            if ( intersectType == QLineF::BoundedIntersection)
                 break;
            p1 = p2;
        }
    }
    topLeft.setX( min< qreal>( srcP.x(), dstP.x()));
    topLeft.setY( min< qreal>( srcP.y(), dstP.y()));
    btmRight.setX( max< qreal>( srcP.x(), dstP.x()));
    btmRight.setY( max< qreal>( srcP.y(), dstP.y()));

    QLineF mainLine = QLineF( srcP, dstP);
        
    if ( mainLine.length() < 1)
        return;

    qreal size = Abs< qreal>(( min< qreal>( Abs< qreal>( mainLine.dx()),
                                            Abs< qreal>(mainLine.dy()))));
    //Stub
    if( size < 2* EdgeControlSize)
    {
        size = 2* EdgeControlSize;
    }
    if( size > 20 * EdgeControlSize)
    {
        size = 20 * EdgeControlSize;
    }
    
    NodeItem *next_pred = NULL;
    NodeItem *next_succ = NULL;

    if ( pred()->isEdgeControl() 
         && isNotNullP( pred()->firstPred()))
    {
        next_pred = pred()->firstPred()->pred()->item();
    } 
    if ( succ()->isEdgeControl()
         && isNotNullP( succ()->firstSucc()))
    {
        next_succ = succ()->firstSucc()->succ()->item();
    } 
    /** Place cp1 */
    if ( isNotNullP( next_pred))
    {
        QPointF p1 = mapFromItem( next_pred, next_pred->boundingRect().center());
        QLineF line( p1, dstCP);
        QPointF cp1_offset = QPointF( (line.dx() * size)/ line.length(),
                                      (line.dy() * size)/ line.length());
        cp1 = srcP + cp1_offset;
    } else
    {
        QPointF cp1_offset = QPointF( (mainLine.dx() * size)/ mainLine.length(),
                                      (mainLine.dy() * size)/ mainLine.length());
        cp1 = srcP + cp1_offset;
    }
    
    topLeft.setX( min< qreal>( topLeft.x(), cp1.x()));
    topLeft.setY( min< qreal>( topLeft.y(), cp1.y()));
    btmRight.setX( max< qreal>( btmRight.x(), cp1.x()));
    btmRight.setY( max< qreal>( btmRight.y(), cp1.y()));

    /** Place cp2 */
    if ( isNotNullP( next_succ))
    {
        QPointF p2 = mapFromItem( next_succ, next_succ->boundingRect().center());
        QLineF line( p2, srcCP);
        QPointF cp2_offset = QPointF( (line.dx() * size)/ line.length(),
                                      (line.dy() * size)/ line.length());
        cp2 = dstP + cp2_offset;
    } else
    {
        QPointF cp2_offset = QPointF( -(mainLine.dx() * size)/ mainLine.length(),
                                      -(mainLine.dy() * size)/ mainLine.length());
        cp2 = dstP + cp2_offset;
    }
    
    topLeft.setX( min< qreal>( topLeft.x(), cp2.x()));
    topLeft.setY( min< qreal>( topLeft.y(), cp2.y()));
    btmRight.setX( max< qreal>( btmRight.x(), cp2.x()));
    btmRight.setY( max< qreal>( btmRight.y(), cp2.y()));

    prepareGeometryChange();
}

QRectF 
EdgeItem::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = arrowSize;

    return QRectF( topLeft,
                   QSizeF( btmRight.x() - topLeft.x(),
                           btmRight.y() - topLeft.y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

QPainterPath 
EdgeItem::shape() const
{
    QPainterPath path( srcP);
        
    if ( srcP == dstP)
        return path;
    QPainterPathStroker stroker;
    path.cubicTo( cp1, cp2, dstP);
    stroker.setWidth( 2);
    return stroker.createStroke( path); 
}

void 
EdgeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    QPointF curr_point;
    QLineF line = QLineF();
    curr_point = srcP;
    QPointF nextToDst = srcP;
    
    line.setP1( nextToDst);
    line.setP2( dstP);
    
    QPainterPath path( srcP);
    QPainterPathStroker stroker;
    path.cubicTo( cp1, cp2, dstP);

    if ( nextToDst == dstP)
        return;

    // Draw the line itself
    if ( option->state & QStyle::State_Selected)
    {
#ifdef SHOW_BACKEDGES
        if ( isInverted())
        {
            painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
#endif
        {
            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
    } else
    {
#ifdef SHOW_BACKEDGES
        if ( isInverted())
        {
            painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
#endif
        {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
    }

    // Draw the arrows if there's enough room
    double angle = ::acos(line.dx() / line.length());
    if ( line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = dstP + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = dstP + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);
    painter->drawPath(path);
    painter->setBrush(Qt::black);
    //painter->drawLine( line);
    if ( !succ()->isEdgeControl())
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2); 
     
#ifdef SHOW_CONTROL_POINTS
    /** For illustrative purposes */
    painter->setPen(QPen(Qt::gray, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine( srcP, dstP);
    painter->drawLine( srcP, cp1);
    painter->drawLine( cp2, dstP);
#endif
}
void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if ( event->button() & Qt::RightButton)
    {
        adjust();
    }
    if ( event->button() == Qt::LeftButton && (flags() & ItemIsSelectable))
    {
        bool multiSelect = ( event->modifiers() & Qt::ControlModifier) != 0;
        if ( !multiSelect)
        {
            if ( !isSelected())
            {
                if (scene())
                    scene()->clearSelection();
                setSelected(true);
            }
        }
    } else if ( !(flags() & ItemIsMovable))
    {
//            event->ignore();
    }
}
void EdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    update();
    QGraphicsItem::mouseReleaseEvent( ev);
}

void EdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{
    if( ev->button() & Qt::LeftButton)
    {
        GNode* new_node = static_cast<GNode *>( edge()->insertNode());
        new_node->setTypeEdgeControl();
        new_node->item()->setPos( ev->pos());
    }
}
